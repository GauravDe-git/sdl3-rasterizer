#pragma once
#include <cstdint>

namespace rast
{
	inline namespace math
	{
		template <typename T>
		struct Rect
		{
			T left	 = T{};
			T top	 = T{};
			T width	 = T{};
			T height = T{};
		};

		using RectI	 = Rect<int32_t>;
		using RectUI = Rect<uint32_t>;
		using RectF	 = Rect<float>;
		using RectD	 = Rect<double>;

	} // namespace math
} // namespace rast