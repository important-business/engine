#include "level.hpp"
#include <iostream>
namespace core
{

bool LevelTile::collides()
{
    switch (m_tilechar)
    {
    case ' ':
        return false;
    case '.':
        return false;
    case ',':
        return false;
    default:
        return true;
    }
}
void LevelTile::get_color(uint8_t& red, uint8_t& blue, uint8_t& green)
{
    switch (m_tilechar)
    {
    case ' ':
        red = 0;
        blue = 0;
        green = 0;
        break;
    case '.':
        red = 0;
        blue = 0;
        green = 0;
        break;
    case ',':
        red = 0;
        blue = 0;
        green = 0;
        break;
    default:
        red = 0;
        blue = 255;
        green = 0;
        break;
    }
}

void LevelTile::set_char(char tilechar)
{
    m_tilechar = tilechar;
}

LevelTile* Level::get(uint16_t x, uint16_t y) const
{
    if ((x > m_size_x) || (y > m_size_y))
    {
        return nullptr;
    }
    return &m_p_tiles[x + y * m_size_x];
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
void Level::load(const char* initialdata, uint16_t size_x, uint16_t size_y)
{
    LevelTile* p_currtile = nullptr;
    for (uint16_t posx = 0; posx < size_x; posx++)
    {
        for (uint16_t posy = 0; posy < size_y; posy++)
        {
            p_currtile = get(posx, posy);
            if (p_currtile != nullptr)
            {
                char c = initialdata[posx + posy * size_x];
                m_sp_logger->info("Loading {} into {},{}", c, posx, posy);
                p_currtile->set_char(c);
            }
            else
            {
                // Maybe do an assert instead
            }
        }
    }
}

void Level::print() const
{
    LevelTile* p_currtile = nullptr;
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
