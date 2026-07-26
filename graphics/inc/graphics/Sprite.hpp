#pragma once
#include "BlendMode.hpp"
#include "Image.hpp"
#include <math/Rect.hpp>

#include <memory>

namespace rast
{
	inline namespace graphics
	{
		class Sprite
		{
		  public:
			Sprite() = default;

			// Construct Sprite from a FilePath
			explicit Sprite(const std::filesystem::path& fileName, const math::RectI& rect, const BlendMode& blendMode = BlendMode{});

			// Construct from an Image
			explicit Sprite(std::shared_ptr<Image> image, const math::RectI& rect, const BlendMode& blendMode = BlendMode{}) noexcept;

			// With or without a Rectangular region
			explicit Sprite(const std::filesystem::path& fileName, const BlendMode& blendMode = BlendMode{});
			explicit Sprite(std::shared_ptr<Image> image, const BlendMode& blendMode = BlendMode{}) noexcept;

			explicit operator bool() const noexcept
			{
				return m_Image != nullptr;
			}

			const math::RectI& getRect() const noexcept
			{
				return m_Rect;
			}

			std::shared_ptr<Image> getImage() const noexcept
			{
				return m_Image;
			}

			const Color& getColor() const noexcept
			{
				return m_Color;
			}

			const BlendMode& getBlendMode() const noexcept
			{
				return m_BlendMode;
			}

		  private:
			std::shared_ptr<Image> m_Image;
			math::RectI			   m_Rect;
			BlendMode			   m_BlendMode;
			Color				   m_Color{Color::White};
		};
	} // namespace graphics
} // namespace rast