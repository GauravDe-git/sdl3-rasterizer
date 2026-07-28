#include "graphics/Tilemap.hpp"

using namespace rast;

TileMap::TileMap(std::shared_ptr<SpriteSheet> spriteSheet, uint32_t columns, uint32_t rows)
	: m_Columns{columns}
	, m_Rows{rows}
	, m_SpriteGrid(static_cast<size_t>(m_Columns) * m_Rows, -1)
	, m_SpriteSheet(std::move(spriteSheet))
{
}

int TileMap::operator()(size_t x, size_t y) const noexcept
{
	if (x < m_Columns && y < m_Rows)
		return m_SpriteGrid[y * m_Columns + x];
	return -1;
}

int& TileMap::operator()(size_t x, size_t y) noexcept
{
	assert ( x < m_Columns);
	assert ( y < m_Rows);
	return m_SpriteGrid[y * m_Columns + x];
}

void TileMap::clear()
{
	std::ranges::fill(m_SpriteGrid, -1);
}

void TileMap::setSpriteGrid(std::span<const int> spriteGrid)
{
	m_SpriteGrid = std::vector(spriteGrid.begin(), spriteGrid.end());
}

/*uint32_t TileMap::getColumns() const noexcept
{
}

uint32_t TileMap::getRows() const noexcept
{
}*/

/*std::shared_ptr<SpriteSheet> TileMap::getSpriteSheet() const noexcept
{
}*/

std::shared_ptr<Image> TileMap::getImage() const noexcept
{
	if (m_SpriteSheet)
	{
		return m_SpriteSheet->getSprite(0).getImage();
	}

	return nullptr;
}

/*const std::vector<int>& TileMap::getSpriteGrid() const noexcept
{
}*/

const BlendMode& TileMap::getBlendMode() const noexcept
{
	if (m_SpriteSheet)
	{
		return m_SpriteSheet->getSprite(0).getBlendMode();
	}

	return BlendMode::Disable;
}

const Sprite& TileMap::getSprite(size_t x, size_t y) const
{
	int spriteId = operator()(x, y);
	if (spriteId >= 0)
		return m_SpriteSheet->getSprite(spriteId);
	static const Sprite emptySprite;
	return emptySprite;
}

uint32_t TileMap::getSpriteWidth() const noexcept
{
	if (m_SpriteSheet)
		return m_SpriteSheet->getSpriteWidth();

	return 0u;
}

uint32_t TileMap::getSpriteHeight() const noexcept
{
	if (m_SpriteSheet)
		return m_SpriteSheet->getSpriteHeight();

	return 0u;
}

uint32_t TileMap::getWidth() const noexcept
{
	return m_Columns * getSpriteWidth();
}

uint32_t TileMap::getHeight() const noexcept
{
	return m_Rows * getSpriteHeight();
}