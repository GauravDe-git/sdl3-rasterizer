#include <graphics/Window.hpp>
#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <numbers>

constexpr int		  SCREEN_WIDTH	= 800;
constexpr int		  SCREEN_HEIGHT = 600;
constexpr const char* TITLE			= "04 - DebugPrimitives";

using namespace rast;

int main()
{
	Window window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);

	Image frameBuffer{SCREEN_WIDTH, SCREEN_HEIGHT};
	frameBuffer.clear({20,20,30});
	
	Rasterizer rasterizer{};
	rasterizer.state.colorTarget = &frameBuffer;
	rasterizer.state.color = Color::White;
	rasterizer.state.blendMode = BlendMode{};	// opaque, blendEnable = false

	const int cx = frameBuffer.getWidth() / 2;
	const int cy = frameBuffer.getHeight() / 2;

	// Fan of lines radiating from center — hits every slope and both directions
	for (int angle = 0; angle < 360; angle += 15)
	{
		float rad = angle * std::numbers::pi_v<float> / 180.0f;
		int   x1  = cx + static_cast<int>(std::cos(rad) * 400); // 400 > half-width
		int   y1  = cy + static_cast<int>(std::sin(rad) * 400); // so lines run off-screen
		rasterizer.drawLine(cx, cy, x1, y1);
	}

	rasterizer.state.fillMode = FillMode::WireFrame;
	rasterizer.state.color	  = Color::Green;
	rasterizer.drawAABB(AABB::fromMinMax({100, 100}, {250, 200}));

	rasterizer.state.fillMode = FillMode::Solid;
	rasterizer.state.color	  = Color::Red;
	rasterizer.drawAABB(AABB::fromMinMax({300, 100}, {450, 200}));

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
		window.present(frameBuffer);
	}

	// cleanup automatic when window goes out of scope

	return 0;
}