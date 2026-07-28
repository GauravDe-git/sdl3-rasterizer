#pragma once

#include "Sprite.hpp"

#include <span>
#include <vector>

namespace rast
{
	inline namespace graphics
	{
		class SpriteSheet
		{
		  public:
			SpriteSheet() = default;

			// Create the spritesheet from a single image
			explicit SpriteSheet(const std::filesystem::path& filePath, std::optional<int> _spriteWidth = {}, std::optional<int> _spriteHeight = {},
								 int padding = 0, int margin = 0, const BlendMode& blendMode = BlendMode{});

			explicit SpriteSheet(const std::shared_ptr<Image>& image, std::optional<int> _spriteWidth = {}, std::optional<int> _spriteHeight = {},
								 int padding = 0, int margin = 0, const BlendMode& blendMode = BlendMode{});

			SpriteSheet(const std::filesystem::path& filePath, std::span<const math::RectI> rects, const BlendMode& blendMode = BlendMode{});
			SpriteSheet(const std::shared_ptr<Image>& image, std::span<const math::RectI> rects, const BlendMode& blendMode = BlendMode{});

			constexpr int getSpriteSize(int imageSize, int numSprites, int padding, int margin)
			{
				return (imageSize - 2 * margin - (numSprites - 1) * padding) / numSprites;
			}

			constexpr int getNumSprites(int imageSize, int spriteSize, int padding, int margin)
			{
				return (imageSize + padding - 2 * margin) / (padding + spriteSize);
			}

			void addSprite( const Sprite& sprite)
			{
				m_Sprites.push_back(sprite);
			}

			int getRows() const noexcept
			{
				return m_Rows;
			}

			int getColumns() const noexcept
			{
				return m_Columns;
			}

			size_t getNumSprites() const noexcept
			{
				return m_Sprites.size();
			}

			int getSpriteWidth(size_t spriteId = 0) const noexcept
			{
				if (spriteId < m_Sprites.size())
					return m_Sprites[spriteId].getWidth();

				return 0;
			}

			int getSpriteHeight(size_t spriteId = 0) const noexcept
			{
				if (spriteId < m_Sprites.size())
					return m_Sprites[spriteId].getHeight();

				return 0;
			}

			const Sprite& getSprite(size_t index) const noexcept;

			const Sprite& operator[](size_t index) const noexcept;

			const Sprite& operator()(size_t i, size_t j) const noexcept;

		  private:
			int m_Columns = 0;
			int m_Rows	  = 0;

			std::vector<Sprite> m_Sprites;
		};
	} // namespace graphics
} // namespace rast