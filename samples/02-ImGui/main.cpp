#include "imgui.h"

#include <graphics/Window.hpp>

constexpr int		  SCREEN_WIDTH	= 800;
constexpr int		  SCREEN_HEIGHT = 600;
constexpr const char* TITLE			= "02 - ImGui";

using namespace rast;

int main()
{
	Window window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);

	while (window)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_EVENT_QUIT:
					window.close();
					break;
				case SDL_EVENT_KEY_DOWN: {
					switch (e.key.key)
					{
						case SDLK_ESCAPE:
							window.close();
							break;
						case SDLK_V:
							window.toggleVsync();
							break;
						case SDLK_RETURN:
							if ((e.key.mod & SDL_KMOD_ALT) != 0)
							{
								case SDLK_F11:
									window.toggleFullscreen();
							}
					}
					break;
				}
			}
		}

		// Clear Screen
		window.clear(154, 206, 235);

		ImGui::ShowDemoWindow();
		window.present();
	}

	// cleanup automatic when window goes out of scope

	return 0;
}