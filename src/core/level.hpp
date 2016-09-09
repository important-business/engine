#ifndef CORE_LEVEL_HPP
#define CORE_LEVEL_HPP

#include "core/logging.hpp"

#include <cstdint>
#include <memory>
namespace core
{

class LevelTile
{
public:
    LevelTile(char tilechar = ' ') : m_tilechar(tilechar)
    {
    }
    bool collides();

    void set_char(char tilechar);

    char m_tilechar;

private:
};

class Level
{
public:
    Level(uint16_t size_x, uint16_t size_y) : m_size_x(size_x), m_size_y(size_y)
    {
        m_p_tiles = new LevelTile[size_x * size_y];
        m_sp_logger = logging_get_logger("level");
    }

    ~Level()
    {
        delete[] m_p_tiles;
        m_p_tiles = nullptr;
    }

    LevelTile* get(uint16_t x, uint16_t y);

    void load(const char* initialdata, uint16_t size_x, uint16_t size_y);
    void print();

private:
    const uint16_t m_size_x, m_size_y;

    LevelTile* m_p_tiles;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* CORE_WORLD_HPP */
