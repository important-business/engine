#ifndef CORE_LEVEL_HPP
#define CORE_LEVEL_HPP

#include "core/logging.hpp"
#include "sdl_wrap/wrap.hpp"

#include <cstdint>
#include <memory>
namespace core
{

// TODO: don't duplicate all the data for each tile
class LevelTile
{
public:
    LevelTile(bool collides = false,
        uint8_t red = 0,
        uint8_t blue = 0,
        uint8_t green = 0)
        : m_tilechar(' '),
          p_texture(nullptr),
          m_collides(collides),
          m_red(red),
          m_blue(blue),
          m_green(green)
    {
    }
    bool collides() const;

    void get_color(uint8_t& red, uint8_t& blue, uint8_t& green) const;

    char m_tilechar;

private:
    std::shared_ptr<sdl_wrap::Texture> p_texture;
    bool m_collides;
    uint8_t m_red, m_blue, m_green;
};

class Level
{
public:
    Level(uint16_t size_x, uint16_t size_y, float scale)
        : m_size_x(size_x), m_size_y(size_y), m_scale(scale)
    {
        m_p_tiles = new const LevelTile*[size_x * size_y];
        m_sp_logger = logging_get_logger("level");
    }

    ~Level()
    {
        delete[] m_p_tiles;
        m_p_tiles = nullptr;
    }

    const LevelTile* get(uint16_t x, uint16_t y) const;

    float get_scale(void) const;

    void get_size(uint16_t& x, uint16_t& y) const;

    void get_tile(
        float world_x, float world_y, uint16_t& tile_x, uint16_t& tile_y) const;

    void load(const std::string* initialdata,
        uint16_t size_x,
        uint16_t size_y,
        std::map<std::string, const LevelTile*> tileset);

    void print() const;

private:
    const uint16_t m_size_x, m_size_y;

    // Number of world units per tile
    const float m_scale;

    const LevelTile** m_p_tiles;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* CORE_WORLD_HPP */
