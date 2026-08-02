#pragma once
#include "Enums.hpp"
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
				Color color = Color::White;   ///< Blend color.
				Image* colorTarget = nullptr; /// Where to draw pixels
				BlendMode blendMode{};		  /// How pixels are blended together
				FillMode  fillMode = FillMode::Solid;
				// Restrict drawing to a rectangular region in the screen
				RectUI clipRect{0u, 0u, UINT_MAX, UINT_MAX};

				Viewport viewport{};		/// Viewport can be used for split screen drawing
			} state;

			/// Clear the color target.
			void clear(std::optional<Color> color = {}) const;

			void drawSprite(const Sprite& sprite, int _x, int _y) const;

			/// ====== Geometry Primitives =========== //

			void drawLine(int x0, int y0, int x1, int y1) const;

			/// Draws an axis-aligned bounding box (AABB).
			/// Required state:
			/// - color
			/// - fillMode
			/// - blendMode (solid fill mode)
			/// - viewport
			void drawAABB(math::AABB aabb) const;

			template <typename T>
			void drawRectangle(const math::Rect<T>& rect) const
			{
				drawAABB(math::AABB::fromRect(rect));
			}

		  private:
			/// Draws a line between two points using an algorithm optimized for lines with a shallow slope (|dy| < |dx|).
			void drawLineLow(int x0, int y0, int x1, int y1) const;

			/// Draws a line between two points using an algorithm optimized for lines with a steep slope (|dy| > |dx|).
			void drawLineHigh(int x0, int y0, int x1, int y1) const;
		};
	} // namespace graphics
} // namespace rast