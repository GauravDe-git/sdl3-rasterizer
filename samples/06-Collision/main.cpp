#include "math/AABB.hpp"

#include <Timer.hpp>
#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

constexpr int		  SCREEN_WIDTH	= 800;
constexpr int		  SCREEN_HEIGHT = 600;
constexpr const char* TITLE			= "06 - Collision";

using namespace rast;

struct Box
{
	glm::vec2 position{0, 0};
	glm::vec2 size{80, 80};

	AABB getAABB() const noexcept
	{
		return AABB::fromMinMax(position, position + size);
	}
};

int main()
{
	Window window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);

	Image frameBuffer{SCREEN_WIDTH, SCREEN_HEIGHT};

	Rasterizer rasterizer{};
	rasterizer.state.colorTarget = &frameBuffer;
	rasterizer.state.blendMode	 = BlendMode{}; // opaque; boxes are solid colors

	// A static "obstacle" box in the middle of the screen.
	Box obstacle;
	obstacle.position = {360, 260};
	obstacle.size	  = {120, 120};

	// A player box the user moves with arrow keys.
	Box player;
	player.position = {100, 100};
	player.size		= {80, 80};

	const float moveSpeed = 220.0f; // pixels per second

	Timer timer;

	while (window)
	{
		SDL_Event e;
		// -- 1. Input for Events
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

			// --- 2. time ---
			timer.tick();
			const float dt = static_cast<float>(timer.elapsedSeconds());

			// --- 3. Update ---
			const bool* keys = SDL_GetKeyboardState(nullptr);
			glm::vec2	move{0, 0};
			if (keys[SDL_SCANCODE_LEFT])
				move.x -= 1.0f;
			if (keys[SDL_SCANCODE_RIGHT])
				move.x += 1.0f;
			if (keys[SDL_SCANCODE_UP])
				move.y -= 1.0f;
			if (keys[SDL_SCANCODE_DOWN])
				move.y += 1.0f;

			player.position += move * moveSpeed * dt;

			// detect overlap with obstacle
			const bool colliding = player.getAABB().intersect(obstacle.getAABB());

			// resolve the collision using overlap()'s minimum translation vector
			// pushing the player box out so it doesnt go into the obstacle
			if (auto mtv = player.getAABB().overlap(obstacle.getAABB()))
			{
				player.position += *mtv; // push out the player
			}

			// keep the player inside the viewport play area
			const Viewport field{40.f, 40.f, SCREEN_WIDTH - 80.0f, SCREEN_HEIGHT - 80.0f};

			// --- 4. Draw

			// 4.1: clear
			frameBuffer.clear(Color{24, 24, 32});

			// restrict drawing to the play area
			rasterizer.state.clipRect = RectUI{
				static_cast<uint32_t>(field.x),
				static_cast<uint32_t>(field.y),
				static_cast<uint32_t>(field.width),
				static_cast<uint32_t>(field.height)};
			rasterizer.state.viewport = field;

			// Draw the obstacle
			rasterizer.state.fillMode = FillMode::Solid;
			rasterizer.state.color	  = Color{90, 90, 110};
			rasterizer.drawAABB(obstacle.getAABB());

			// Draw the player: solid, green normally, red when colliding.
			rasterizer.state.fillMode = FillMode::Solid;
			rasterizer.state.color	  = colliding ? Color::Red : Color::Green;
			rasterizer.drawAABB(player.getAABB());

			// Draw a wireframe outline of the play-field so the region is visible.
			rasterizer.state.clipRect = RectUI{0u, 0u, UINT_MAX, UINT_MAX}; // unclip for the border
			rasterizer.state.fillMode = FillMode::WireFrame;
			rasterizer.state.color	  = Color{70, 70, 90};
			rasterizer.drawAABB(AABB::fromMinMax(
				{field.x, field.y},
				{field.x + field.width, field.y + field.height}));

			window.present(frameBuffer);
		}
	}
	// cleanup automatic when window goes out of scope
	return 0;
}