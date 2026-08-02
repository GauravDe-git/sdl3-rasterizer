#include <graphics/ResourceManager.hpp>

#include "graphics/SpriteSheet.hpp"

#include <unordered_map>

using namespace rast::graphics;

namespace
{
	// Image store. Anonymous namespace for "private" variables.
	
	using ImageMap = std::unordered_map<std::filesystem::path, std::shared_ptr<Image>>;

	ImageMap& im()
	{
		static ImageMap map;
		return map;
	}
}

std::shared_ptr<Image> ResourceManager::loadImage(const std::filesystem::path& filePath)
{
	auto& imageMap = im();

	const auto iter = imageMap.find( filePath );
	if (iter == imageMap.end())
	{
		auto image = std::make_shared<Image>( filePath );
		imageMap.insert({filePath, image});
		return image;
	}

	return iter->second;
}

void ResourceManager::clearImages()
{
	im().clear();
}

std::shared_ptr<SpriteSheet> ResourceManager::loadSpriteSheet(const std::filesystem::path& filePath, std::optional<int> spriteWidth, std::optional<int> spriteHeight, int padding, int margin, const BlendMode& blendMode)
{
	auto image = loadImage(filePath);
	return std::make_shared<SpriteSheet>(image, spriteWidth, spriteHeight, padding, margin, blendMode);
}

std::shared_ptr<SpriteSheet> ResourceManager::loadSpriteSheet(const std::filesystem::path& filePath, std::span<const math::RectI> rects, const BlendMode& blendMode)
{
	auto image = loadImage(filePath);
	return std::make_shared<SpriteSheet>(image, rects, blendMode);
}

void ResourceManager::clear()
{
	clearImages();
}