#include <graphics/ResourceManager.hpp>

#include <unordered_map>

using namespace rast::graphics;

namespace
{
	// Image store. Anonymous namespace for "private" variables.
	
	std::unordered_map<std::filesystem::path, std::shared_ptr<Image>> g_ImageMap;
}

std::shared_ptr<Image> ResourceManager::loadImage(const std::filesystem::path& filePath)
{
	const auto iter = g_ImageMap.find( filePath );
	if (iter == g_ImageMap.end())
	{
		auto image = std::make_shared<Image>( filePath );
		g_ImageMap.insert({filePath, image});
		return image;
	}

	return iter->second;
}