#include "graphics/Sprite.hpp"
#include "graphics/ResourceManager.hpp"

using namespace rast;

Sprite::Sprite(const std::filesystem::path& fileName, const BlendMode& blendMode)
	: m_Image{ResourceManager::loadImage(fileName)}
	, m_BlendMode{blendMode}
{
	if (m_Image)
		m_Rect = RectI{0, 0, m_Image->getWidth(), m_Image->getHeight()};
}

Sprite::Sprite(const std::filesystem::path& fileName, const math::RectI& rect, const BlendMode& blendMode)
	: m_Image{ResourceManager::loadImage(fileName)}
	, m_Rect{rect}
	, m_BlendMode{blendMode}
{
}

Sprite::Sprite(std::shared_ptr<Image> image, const math::RectI& rect, const BlendMode& blendMode) noexcept
	: m_Image{std::move(image)}
	, m_Rect{rect}
	, m_BlendMode{blendMode}
{
}

Sprite::Sprite(std::shared_ptr<Image> image, const BlendMode& blendMode) noexcept
	: m_Image{std::move(image)}
	, m_Rect{m_Image ? math::RectI{0, 0, m_Image->getWidth(), m_Image->getHeight()}
					 : math::RectI{}}
	, m_BlendMode{blendMode}
{
}