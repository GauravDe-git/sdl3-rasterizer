#pragma once

#include <algorithm>
#include <cstdint>

namespace rast
{
	inline namespace graphics
	{

		union Color {
			constexpr Color() noexcept
				: channels{0, 0, 0, 255}
			{
			}

			constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u) noexcept
				: channels{r, g, b, a}
			{
			}

			constexpr explicit Color(uint32_t rgba) noexcept
				: rgba{rgba}
			{
			}

			~Color() noexcept						 = default;
			constexpr Color(const Color&) noexcept	 = default;
			constexpr Color(Color&&) noexcept		 = default;
			constexpr Color& operator=(const Color&) = default;
			constexpr Color& operator=(Color&&)		 = default;

			constexpr bool operator==(const Color& rhs) const noexcept
			{
				return rgba == rhs.rgba;
			}

			Color  operator+(const Color& rhs) const noexcept;
			Color& operator+=(const Color& rhs) noexcept;
			Color  operator-(const Color& rhs) const noexcept;
			Color& operator-=(const Color& rhs) noexcept;
			Color  operator*(const Color& rhs) const noexcept;
			Color& operator*=(const Color& rhs) noexcept;
			Color  operator*(float rhs) const noexcept;
			Color& operator*=(float rhs) noexcept;

			uint32_t rgba;
			struct Channels
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			} channels;

			// few basic colors
			static const Color Black;
			static const Color White;
			static const Color Red;
			static const Color Green;
			static const Color Blue;
			static const Color Yellow;
			static const Color Cyan;
			static const Color Magenta;
		};

		// some free functions needed by BlendMode
		inline Color min(const Color& c1, const Color& c2) noexcept
		{
			return {
				std::min(c1.channels.r, c2.channels.r),
				std::min(c1.channels.g, c2.channels.g),
				std::min(c1.channels.b, c2.channels.b),
				std::min(c1.channels.a, c2.channels.a),
			};
		}
		inline Color max(const Color& c1, const Color& c2) noexcept
		{
			return {
				std::max(c1.channels.r, c2.channels.r),
				std::max(c1.channels.g, c2.channels.g),
				std::max(c1.channels.b, c2.channels.b),
				std::max(c1.channels.a, c2.channels.a),
			};			
		}

		static_assert(sizeof(Color) == sizeof(uint32_t));

	} // namespace graphics
} // namespace rast