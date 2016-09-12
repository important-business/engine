#include "level.hpp"
#include <iostream>
namespace core
{

bool LevelTile::collides() const
{
    return m_collides;
}
void LevelTile::get_color(uint8_t& red, uint8_t& blue, uint8_t& green) const
{
    red = m_red;
    blue = m_blue;
    green = m_green;
}

const LevelTile* Level::get(uint16_t x, uint16_t y) const
{
    if ((x > m_size_x) || (y > m_size_y))
    {
        return nullptr;
    }
    return m_p_tiles[x + y * m_size_x];
}

float Level::get_scale() const
{
    return m_scale;
}

void Level::get_size(uint16_t& x, uint16_t& y) const
{
    x = m_size_x;
    y = m_size_y;
}

void Level::get_tile(
    float world_x, float world_y, uint16_t& tile_x, uint16_t& tile_y) const
{
    // TODO(Keegan): Don't think it's actually quite this simple
    tile_x = world_x * m_scale;
    tile_y = world_y * m_scale;
}
void Level::load(const std::string* initialdata,
    uint16_t size_x,
    uint16_t size_y,
    std::map<std::string, const LevelTile*> tileset)
{
    const LevelTile* p_currtile = nullptr;
    for (uint16_t posx = 0; posx < size_x; posx++)
    {
        for (uint16_t posy = 0; posy < size_y; posy++)
        {
            auto tiledata = initialdata[posx + posy * size_x];
            m_sp_logger->info("Loading {} into {},{}", tiledata, posx, posy);
            // TODO: Use universal setter method instead
            m_p_tiles[posx + posy * m_size_y] = tileset[tiledata];
        }
    }
}

void Level::print() const
{
    const LevelTile* p_currtile = nullptr;
    for (uint16_t posy = 0; posy < m_size_y; posy++)
    {
        for (uint16_t posx = 0; posx < m_size_x; posx++)
        {
            p_currtile = get(posx, posy);
            if (p_currtile != nullptr)
            {
                std::cout << '[' << p_currtile->m_tilechar << ']';
            }
            else
            {
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

} // namespace core
