#include "graphics/Rasterizer.hpp"

using namespace rast;

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
	const int clipRight	 = std::max(static_cast<int>(dstAABB.max.x), _x + size.x - 1);
	const int clipBottom = std::max(static_cast<int>(dstAABB.max.y), _y + size.y - 1);

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