#pragma once

#include "SDL3/SDL_render.h"
#include <SDL3/SDL_events.h>

#include "glm/vec2.hpp"

#include <string_view>

struct SDL_Window;
struct SDL_Renderer;
struct ImGuiContext;

namespace rast
{
	inline namespace graphics
	{
		struct Image;

		class Window
		{
		  public:
			Window() = default;
			~Window();

			Window(std::string_view title, int width, int height, bool fullscreen = false);
			Window(const Window&) = delete;
			Window(Window&& window) noexcept;

			Window& operator=(const Window&) = delete;
			Window& operator=(Window&&) noexcept;

			explicit operator bool() const; // Allows the window to be tested as a boolean.

			void create(std::string_view title, int width, int height, bool fullscreen = false);

			void destroy();

			void close(); // hide the window

			bool isValid() const;

			void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const;

			void present() const;

			void resize(int width, int height);

			void setFullScreen(bool fullscreen);

			void toggleFullscreen();

			bool isFullscreen() const noexcept;

			void setVsync(bool vSync);

			void toggleVsync();

			bool isVsync() const noexcept;

			// Set this Window as the current ImGui context
			bool setCurrent() const;

			void present(const Image& image);

			glm::vec2 clientToImage( float x, float y, const Image& image) const noexcept;

		  private:
			static bool SDLCALL eventWatch(void* userdata, SDL_Event* event);

			// Begin a new ImGui frame
			void beginFrame() const;

			// Each Window has it's own ImGui context
			ImGuiContext* m_ImGuiContext = nullptr;

			SDL_Window*	  m_Window	   = nullptr;
			SDL_Renderer* m_Renderer   = nullptr;
			int			  m_Width	   = -1;
			int			  m_Height	   = -1;
			bool		  m_Fullscreen = false;
			bool		  m_VSync	   = true;
			bool		  m_Close	   = false;

			SDL_Texture* m_Texture = nullptr;
		};



	} // namespace graphics

} // namespace rast