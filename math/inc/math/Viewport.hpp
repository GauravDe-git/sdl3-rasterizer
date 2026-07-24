#pragma once

#include <limits>

namespace rast
{
	inline namespace math
	{
		struct Viewport
		{
			explicit Viewport(float x = 0.f, float y = 0.f,
							  float width	 = std::numeric_limits<float>::max(),
							  float height	 = std::numeric_limits<float>::max(),
							  float minDepth = 0.f, float maxDepth = 1.f)
				: x{x}, y{y}, width{width}, height{height}, minDepth{minDepth}, maxDepth{maxDepth}
			{
			}

			float x		   = 0.f;								/// X position of the left side of viewport
			float y		   = 0.f;								/// Y position of the top side of viewport
			float width	   = std::numeric_limits<float>::max(); /// Width of viewport in Pixels
			float height   = std::numeric_limits<float>::max(); /// Height of viewport in Pixels
			float minDepth = 0.f;								/// Minimum depth of viewport in range 0-1
			float maxDepth = 1.f;								/// Maximum depth of viewport in range 0-1
		};
	} // namespace math
} // namespace rast