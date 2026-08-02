#pragma once
#include <cstdint>

namespace rast
{
	inline namespace graphics
	{
		enum class FillMode : std::uint8_t
		{
			WireFrame, ///< Outline only (four lines).
			Solid	   ///< Filled interior.
		};
	}
} // namespace rast