#include <graphics/Image.hpp>
#include <graphics/Window.hpp>

#include "SDL3/SDL_timer.h"

#include <iostream>
#include <numbers>
#include <utility>

constexpr int		  SCREEN_WIDTH	= 800;
constexpr int		  SCREEN_HEIGHT = 600;
constexpr const char* TITLE			= "03 - Images";

using namespace rast;

void drawGradient(Image& img)
{
	const int w = img.getWidth();
	const int h = img.getHeight();
	for (uint32_t y = 0; std::cmp_less(y, h); ++y)
	{
		for (uint32_t x = 0; std::cmp_less(x, w); ++x)
		{
			uint8_t r = static_cast<uint8_t>(x * 255 / w); // 0-255 left to right
			uint8_t g = static_cast<uint8_t>(y * 255 / h);
			img.plot<false, false>(x, y, Color{r, g, 128});
		}
	}
}

void drawCircles(Image& img)
{
	const int w = img.getWidth();
	const int h = img.getHeight();
	const float cx = w / 2.f;	// Center
	const float cy = h / 2.f;

	for (uint32_t y = 0; std::cmp_less(y, h); ++y)
	{
		for (uint32_t x = 0; std::cmp_less(x, w); ++x)
		{
			float dx = x - cx;
			float dy = y - cy;
			float dist = std::sqrt(dx * dx + dy * dy);	// Distance from center

			// Turn distance into a repeating 0..255 value, it creates rings every 20pixels
			uint8_t v = static_cast<uint8_t>((std::sin(dist * 0.1f) * 0.5f + 0.5f) * 255.f);

			img.plot<false,false>(x , y, Color{v,v,v});		// gray scale rings
		}
	}
}

void drawPlasma(Image& img, float time)
{
	const int w = img.getWidth();
	const int h = img.getHeight();

	for (uint32_t y = 0; std::cmp_less(y, h); ++y)
		for (uint32_t x = 0; std::cmp_less(x, w); ++x)
		{
			const float fx = x * 0.02f;
			const float fy = y * 0.02f;

			// sum of few sin waves
			float v = std::sin(fx + time) + std::sin(fy + time) + std::sin((fx + fy) * 0.5f + time) + std::sin(std::sqrt(fx * fx + fy * fy) + time);

			v = v * 0.25f;		 // average the 4 waves → roughly -1..1
			v = v * 0.5f + 0.5f; // remap to 0..1

			// Map the single value to RGB via phase-shifted sines for color
			uint8_t r = static_cast<uint8_t>((std::sin(v * std::numbers::pi_v<float> * 2 + 0.0f) * 0.5f + 0.5f) * 255);
			uint8_t g = static_cast<uint8_t>((std::sin(v * std::numbers::pi_v<float> * 2 + 2.0f) * 0.5f + 0.5f) * 255);
			uint8_t b = static_cast<uint8_t>((std::sin(v * std::numbers::pi_v<float> * 2 + 4.0f) * 0.5f + 0.5f) * 255);

			img.plot<false, false>(x, y, Color{r, g, b});
		}
}

int main()
{
	Window window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);

	std::cout << "Working dir: "
			  << std::filesystem::current_path() << std::endl;

	//Image jacco{"jacco.jpg"};
	Image canvas{SCREEN_WIDTH, SCREEN_HEIGHT};
	
	drawGradient(canvas);
	//drawCircles(canvas);

	uint64_t start =  SDL_GetTicks();

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

		float time = (SDL_GetTicks() - start) / 1000.0f; // seconds since start
		//drawPlasma(canvas, time); 

		// Clear Screen
		window.clear(154, 206, 235);
		window.present(canvas);
	}

	// cleanup automatic when window goes out of scope

	return 0;
}