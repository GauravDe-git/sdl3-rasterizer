#pragma once

#include "SpriteSheet.hpp"

namespace rast
{
	inline namespace graphics
	{
		class TileMap
		{
		  public:
			TileMap() = default;
			TileMap(std::shared_ptr<SpriteSheet> spriteSheet, uint32_t columns, uint32_t rows);

			// If change to C++23 then may use []

			int	 operator()(size_t x, size_t y) const noexcept;
			int& operator()(size_t x, size_t y) noexcept;

			// clear the whole tilemap (set every value to -1)
			void clear();

			// set every sprite ID at once
			void setSpriteGrid(std::span<const int> spriteGrid);

			//uint32_t					 getColumns() const noexcept;
			//uint32_t					 getRows() const noexcept;
			//std::shared_ptr<SpriteSheet> getSpriteSheet() const noexcept;
			std::shared_ptr<Image>		 getImage() const noexcept;
			//const std::vector<int>&		 getSpriteGrid() const noexcept;
			const BlendMode&			 getBlendMode() const noexcept;
			const Sprite&				 getSprite(size_t x, size_t y) const;
			uint32_t					 getSpriteWidth() const noexcept;
			uint32_t					 getSpriteHeight() const noexcept;
			uint32_t					 getWidth() const noexcept;
			uint32_t					 getHeight() const noexcept;

		  private:
			uint32_t					 m_Columns = 0u;
			uint32_t					 m_Rows	   = 0u;
			std::vector<int>			 m_SpriteGrid;
			std::shared_ptr<SpriteSheet> m_SpriteSheet;

			// Sprite id of "- 1" means no Sprite
		};
	} // namespace graphics
} // namespace rast