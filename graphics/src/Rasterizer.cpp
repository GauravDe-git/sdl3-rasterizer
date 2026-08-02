#include "graphics/Rasterizer.hpp"

using namespace rast;

void Rasterizer::clear(std::optional<Color> color) const
{
	if (Image* image = state.colorTarget)
	{
		image->clear(color.value_or(state.color));
	}
}

void Rasterizer::drawSprite(const Sprite& sprite, int _x, int _y) const
{
	const Image* srcImage = sprite.getImage().get();
	Image*		 dstImage = state.colorTarget;

	if (!srcImage || !dstImage)
		return;

	const AABB		 clipAABB = AABB::fromRect(state.clipRect);
	const AABB		 dstAABB  = dstImage->getAABB().clamped(clipAABB);
	const glm::ivec2 size	  = sprite.getSize();

	const int clipLeft	 = std::max(static_cast<int>(dstAABB.min.x), _x);
	const int clipTop	 = std::max(static_cast<int>(dstAABB.min.y), _y);
	const int clipRight	 = std::min(static_cast<int>(dstAABB.max.x), _x + size.x - 1);
	const int clipBottom = std::min(static_cast<int>(dstAABB.max.y), _y + size.y - 1);

	// If the clip rectangle is invalid, then sprite is off-screen
	if (clipLeft >= clipRight || clipTop >= clipBottom)
		return;

	// Adjust sprite UV based on clipping
	glm::ivec2 uv = sprite.getUV();
	uv.x += clipLeft - _x;
	uv.y += clipTop - _y;

	const Color		color	  = sprite.getColor();
	const BlendMode blendMode = sprite.getBlendMode();
	int				sW		  = srcImage->getWidth(); // Source image width.
	const Color*	src		  = srcImage->data();

	int	   dW  = dstImage->getWidth(); // Dst image width
	Color* dst = dstImage->data();

	// Rasterize the sprite
	for (int y = clipTop; y <= clipBottom; ++y)
	{
		int v = uv.y + (y - clipTop);
		for (int x = clipLeft; x <= clipRight; ++x)
		{
			int	  u			= uv.x + (x - clipLeft);
			Color sC		= src[v * sW + u] * color;
			Color dC		= dst[y * dW + x];
			dst[y * dW + x] = blendMode.Blend(sC, dC);
		}
	}
}

void Rasterizer::drawLine(int x0, int y0, int x1, int y1) const
{
	if (std::abs(y1 - y0) < std::abs(x1 - x0))
	{
		if (x0 > x1)
			drawLineLow(x1, y1, x0, y0);
		else
			drawLineLow(x0, y0, x1, y1);
	}
	else
	{
		if (y0 > y1)
			drawLineHigh(x1, y1, x0, y0);
		else
			drawLineHigh(x0, y0, x1, y1);
	}
}

void Rasterizer::drawAABB(math::AABB aabb) const
{
	Image*	 image	  = state.colorTarget;
	Viewport viewport = state.viewport;

	if (!image)
		return;

	AABB imageAABB = image->getAABB();
	imageAABB.clamp(AABB::fromViewport(viewport));

	if (!aabb.intersect(imageAABB))
		return;

	switch (state.fillMode)
	{
		case FillMode::WireFrame:
			drawLine( static_cast<int>(aabb.min.x), static_cast<int>(aabb.min.y), static_cast<int>(aabb.max.x), static_cast<int>(aabb.min.y));
			drawLine(static_cast<int>(aabb.max.x), static_cast<int>(aabb.min.y), static_cast<int>(aabb.max.x), static_cast<int>(aabb.max.y));
			drawLine(static_cast<int>(aabb.max.x), static_cast<int>(aabb.max.y), static_cast<int>(aabb.min.x), static_cast<int>(aabb.max.y));
			drawLine(static_cast<int>(aabb.min.x), static_cast<int>(aabb.max.y), static_cast<int>(aabb.min.x), static_cast<int>(aabb.min.y));
			break;
		case FillMode::Solid:
			aabb.clamp(imageAABB);
			for (int y = static_cast<int>(aabb.min.y); y <= static_cast<int>(aabb.max.y); ++y)
			{
				for (int x = static_cast<int>(aabb.min.x); x <= static_cast<int>(aabb.max.x); ++x)
				{
					image->plot<false>(x, y, state.color, state.blendMode);
				}
			}
			break;
	}
}

void Rasterizer::drawLineLow(int x0, int y0, int x1, int y1) const
{
	Image*	  image		= state.colorTarget;
	BlendMode blendMode = state.blendMode;

	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;

	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}

	int D = 2 * dy - dx;
	int y = y0;

	for (int x = x0; x <= x1; ++x)
	{
		image->plot<true>(x, y, state.color, blendMode);

		if (D > 0)
		{
			y += yi;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}
}

void Rasterizer::drawLineHigh(int x0, int y0, int x1, int y1) const
{
	Image*	  image		= state.colorTarget;
	BlendMode blendMode = state.blendMode;

	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;

	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}

	int D = 2 * dx - dy;
	int x = x0;

	for (int y = y0; y <= y1; ++y)
	{
		image->plot<true>(x, y, state.color, blendMode);

		if (D > 0)
		{
			x += xi;
			D -= 2 * dy;
		}

		D += 2 * dx;
	}
}