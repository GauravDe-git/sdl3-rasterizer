#pragma once
#include "SpriteSheet.hpp"

namespace rast
{
	inline namespace graphics
	{
		class SpriteAnimation
		{
		  public:
			SpriteAnimation() = default;

			explicit SpriteAnimation(std::shared_ptr<SpriteSheet> spriteSheet, float fps = 30.f, std::span<const int> frames = {});

			void update(float deltaTime) noexcept;

			// Gets total animation duration in seconds
			float getDuration() const noexcept;

			// If animation has finished playing
			bool isDone() const noexcept;

			void reset() noexcept;

			// Get sprite at a specific point in time
			const Sprite& at(float time) const noexcept;

			// Access a certain frame in the animation
			const Sprite& operator[](size_t frame) const noexcept;

			// Implicitly convert to sprite, treat sprite-anim as a sprite
			operator const Sprite&() const noexcept;

		  private:
			std::shared_ptr<SpriteSheet> m_SpriteSheet;

			std::vector<int> m_Frames;

			float m_FrameRate = 30.f;

			// Current animation time
			float m_Time = 0.f;
		};
	} // namespace graphics
} // namespace rast