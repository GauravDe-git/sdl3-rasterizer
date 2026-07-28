#pragma once
#include "Sprite.hpp"
#include <math/Rect.hpp>

namespace rast
{
	inline namespace graphics
	{
		class Rasterizer
		{
		public:
			struct State
			{
				// Where to draw pixels
				Image* colorTarget = nullptr;

				// Restrict drawing to a rectangular region in the screen
				RectUI clipRect {0u, 0u, UINT_MAX, UINT_MAX};
			} state ;

			void drawSprite(const Sprite& sprite, int _x, int _y) const;

		};
	} // namespace graphics
} // namespace rast