#include <graphics/Window.hpp>

#include <Timer.hpp>
#include "graphics/Rasterizer.hpp"

#include <graphics/SpriteAnimation.hpp>
#include <graphics/ResourceManager.hpp>

constexpr int		  SCREEN_WIDTH	= 800;
constexpr int		  SCREEN_HEIGHT = 600;
constexpr const char* TITLE			= "05 - Sprites";

using namespace rast;

int main()
{
	Window window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);

	Image framebuffer{SCREEN_WIDTH, SCREEN_HEIGHT};

	Rasterizer rasterizer{};
	rasterizer.state.colorTarget = &framebuffer;

	// --- SpriteSheet: slice 1 image into grid of frames --- //
	// Pixel adventure Frog: horizontal 32x32 frames
	auto runSheet = std::make_shared<SpriteSheet>("run.png", 32, 32, 0, 0, BlendMode::AlphaBlend);

	// --- SpriteAnimation --- //
	SpriteAnimation runAnim{runSheet};

	// --- Static Sprite --- //
	const Sprite& firstFrame = runSheet->getSprite(0);

	Timer timer;

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

		// Advance the clock
		timer.tick();
		const float dT = static_cast<float>(timer.elapsedSeconds());

		// Clear Screen
		//window.clear(154, 206, 235);
		// using the framebuffer instead:
		framebuffer.clear(Color{30, 30, 40});
		
		// Draw Loop
		rasterizer.drawSprite(firstFrame, 60, 60);

		for (size_t i = 0; i < runSheet->getNumSprites(); ++i)
		{
			const int x  = 60 + static_cast<int>(i) * 40; // 40 pixels apart
			rasterizer.drawSprite(runSheet->getSprite(i), x, 160);
		}

		runAnim.update(dT);

		rasterizer.drawSprite(runAnim, 380, 300);
		
		window.present(framebuffer);
	}

	// cleanup automatic when window goes out of scope

	return 0;
}