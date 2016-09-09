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

void LevelTile::set_char(char tilechar)
{
    m_tilechar = tilechar;
}

LevelTile* Level::get(uint16_t x, uint16_t y)
{
    if ((x > m_size_x) || (y > m_size_y))
    {
        return nullptr;
    }
    return &m_p_tiles[x + y * m_size_x];
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

void Level::print()
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
