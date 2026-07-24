#include <graphics/Window.hpp>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_internal.h>	// for ImGuiContext

#include <stdexcept>
#include <utility> // for std::exchange

using namespace rast;

struct SDL_Context
{
	static SDL_Context& get()
	{
		static SDL_Context instance;
		return instance;
	}

  private:
	SDL_Context()
	{
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL: %s", SDL_GetError());
			throw std::runtime_error(SDL_GetError());
		}
	}

	~SDL_Context()
	{
		SDL_Quit();
	}
};

Window::~Window()
{
	destroy();
}

Window::Window(std::string_view title, int width, int height, bool fullscreen)
{
	create(title, width, height, fullscreen);
}

Window::Window(Window&& window) noexcept
	: m_Window(std::exchange(window.m_Window, nullptr)), 
	m_Renderer(std::exchange(window.m_Renderer, nullptr)),
	m_ImGuiContext(std::exchange(window.m_ImGuiContext, nullptr)),
	m_Width(std::exchange(window.m_Width, -1)), 
	m_Height(std::exchange(window.m_Height, -1)), 
	m_Fullscreen(std::exchange(window.m_Fullscreen, false)),
	m_VSync(std::exchange(window.m_VSync, true))
{
}

Window& Window::operator=(Window&& window) noexcept
{
	if ( this == &window)
		return *this;

	m_Window = std::exchange(window.m_Window, nullptr);
	m_Renderer = std::exchange(window.m_Renderer, nullptr);
	m_ImGuiContext = std::exchange(window.m_ImGuiContext, nullptr);
	m_Width = std::exchange(window.m_Width, -1);
	m_Height = std::exchange(window.m_Height, -1);
	m_Fullscreen = std::exchange(window.m_Fullscreen, false);
	m_VSync = std::exchange(window.m_VSync, true);

	return *this;
}

Window::operator bool() const
{
	return isValid();
}

void Window::create(std::string_view title, int width, int height, bool fullscreen)
{
	static SDL_Context& context = SDL_Context::get();

	if (m_Window)
		destroy();

	m_Fullscreen = fullscreen;
	SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
	flags |= m_Fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	if (!SDL_CreateWindowAndRenderer(title.data(), width, height, flags, &m_Window, &m_Renderer))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s",SDL_GetError());
		throw std::runtime_error(SDL_GetError());
	}

	if ( !SDL_AddEventWatch( &Window::eventWatch, this))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to add event watch: %s", SDL_GetError());
		throw std::runtime_error(SDL_GetError());
	}

	// Enable VSync
	SDL_SetRenderVSync(m_Renderer, m_VSync ? 1 : 0);

	resize(width, height);

	// ===== Create the ImGui Context ====== //

	// Each window has its own ImGui context
	m_ImGuiContext = ImGui::CreateContext();

	// Name the context the same as the Window title.
	// Note: Use ImGui::SetContextName when it gets merged to the docking branch.
	ImStrncpy(m_ImGuiContext->ContextName, title.data(), IM_ARRAYSIZE(m_ImGuiContext->ContextName));
	ImGui::SetCurrentContext(m_ImGuiContext);	// Set the current context to this window's ImGui context

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Keyboard
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Gamepad
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Multiple window/viewports
	// Note: SDL3 backend doesn't currently support multi-viewports, but maybe soon ?!

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	float primaryDisplayScale = SDL_GetDisplayContentScale( SDL_GetDisplayForWindow(m_Window) );
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(primaryDisplayScale);
	style.FontScaleDpi = primaryDisplayScale;
	io.ConfigDpiScaleViewports = true;

	// Setup Platform/Renderer backends for ImGui.
	ImGui_ImplSDL3_InitForSDLRenderer( m_Window, m_Renderer); 
	ImGui_ImplSDLRenderer3_Init(m_Renderer);

	beginFrame();
}

void Window::destroy()
{
	SDL_RemoveEventWatch( &Window::eventWatch, this);

	// Destroy ImGui Context
	if (m_ImGuiContext)
	{
		ImGuiContext* previousContext = ImGui::GetCurrentContext();
		if (previousContext == m_ImGuiContext)
			previousContext = nullptr;
		ImGui::SetCurrentContext(m_ImGuiContext);
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext(m_ImGuiContext);
		m_ImGuiContext = nullptr;

		// Restore previous context
		ImGui::SetCurrentContext(previousContext);
	}


	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);

	m_Window	   = nullptr;
	m_Renderer   = nullptr;
	m_Width	   = -1;
	m_Height	   = -1;
	m_Fullscreen = false;
	m_VSync	   = true;
}

void Window::close()
{
	if (!m_Window)
		return;

	m_Close = true;

	SDL_HideWindow(m_Window); // Just hide, destroy later
}

bool Window::isValid() const
{
	return m_Window && !m_Close;
}

void Window::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
{
	if (!m_Renderer)
		return;

	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	SDL_RenderClear( m_Renderer ); 
}

void Window::present() const
{
	if ( !m_ImGuiContext || !m_Renderer)
		return;

	ImGui::SetCurrentContext(m_ImGuiContext);

	// ImGui rendering
	ImGui::Render(); 

	// Update for multiple viewports
	// Multi-viewports not yet supported by SDL3 backend though
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);

	if (!SDL_RenderPresent(m_Renderer))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to present: %s", SDL_GetError());
		// throw an exception or maybe not meh
	}

	beginFrame();
}

void Window::resize(int width, int height)
{
	if (m_Width != width || m_Height != height)
	{
		width = std::max(1, width);
		height = std::max(1, height);

		if (!SDL_SetWindowSize(m_Window, width, height))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to resize window: %s", SDL_GetError());
			return;
		}

		m_Width = width;
		m_Height = height;
	}
}

void Window::setFullScreen(bool fullscreen)
{
	if(m_Window)
	{
		SDL_SetWindowFullscreen(m_Window, fullscreen);
		m_Fullscreen = fullscreen;
	}
}

void Window::toggleFullscreen()
{
	setFullScreen( !m_Fullscreen );
}

bool Window::isFullscreen() const noexcept
{
	return m_Fullscreen;
}

void Window::setVsync(bool vSync)
{
	if (m_Renderer)
	{
		SDL_SetRenderVSync(m_Renderer, vSync ? 1 : 0);
		m_VSync = vSync;
	}
}

void Window::toggleVsync()
{
	setVsync( !m_VSync);
}

bool Window::isVsync() const noexcept
{
	return m_VSync;
}

bool Window::setCurrent() const
{
	if (m_ImGuiContext)
	{
		ImGui::SetCurrentContext(m_ImGuiContext);
		return true;
	}
	return false;
}

bool SDLCALL Window::eventWatch(void* userdata, SDL_Event* event)
{
	Window* self = static_cast<Window*>(userdata);

	// A RAII helper to switch and restore ImGui contexts.
	struct ContextSwitcher
	{
		ContextSwitcher(ImGuiContext* newContext)
		{
			previousContext = ImGui::GetCurrentContext();
			ImGui::SetCurrentContext( newContext );
		}

		// Restore the previous context when the switcher goes out of scope.
		~ContextSwitcher()
		{
			ImGui::SetCurrentContext( previousContext );
		}

		ImGuiContext* previousContext;
	} switcher(self->m_ImGuiContext);
	
	ImGui_ImplSDL3_ProcessEvent( event );

	switch (event->type)
	{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			if (event->window.windowID == SDL_GetWindowID(self->m_Window))
			{
				self->close();
			}
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			if (event->window.windowID == SDL_GetWindowID(self->m_Window))
			{
				self->m_Width = event->window.data1;
				self->m_Height = event->window.data2;
			}
			break;
		case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
		case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: 
		{
			SDL_DisplayID id = SDL_GetDisplayForWindow(self->m_Window);
			float scale = SDL_GetDisplayContentScale(id);
			// Change imGui styles for the new display scale.
			ImGuiStyle style;
			ImGui::StyleColorsDark( &style );
			style.ScaleAllSizes( scale);
			style.FontScaleDpi = scale;
			ImGui::GetStyle() = style;
		}
		break;
	}
	return true;
}

void Window::beginFrame() const
{
	// Start the Dear ImGui frame
	ImGui::SetCurrentContext( m_ImGuiContext);
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}
