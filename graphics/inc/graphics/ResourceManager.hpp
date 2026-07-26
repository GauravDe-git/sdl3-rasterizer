#pragma once
#include "Image.hpp"

#include <filesystem>
#include <memory>

namespace rast
{
	inline namespace graphics
	{
		namespace ResourceManager
		{
			std::shared_ptr<Image> loadImage(const std::filesystem::path& filePath);



		}
	} // namespace graphics
} // namespace rast