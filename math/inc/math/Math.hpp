#pragma once

namespace rast
{
	inline namespace math
	{
		template <typename T>
		constexpr T min(T a, T b) noexcept
		{
			return a < b ? a : b;
		}

		template <typename T>
		constexpr T max(T a, T b) noexcept
		{
			return a > b ? a : b;
		}

		template<typename T>
		constexpr T clamp(T v, T minVal, T maxVal) noexcept
		{
			return min( max(v, minVal), maxVal );
		}

	} // namespace math
} // namespace rast