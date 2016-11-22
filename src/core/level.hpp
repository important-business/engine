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
          m_sp_texture(nullptr),
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
    std::shared_ptr<sdl_wrap::Texture> m_sp_texture;
    bool m_collides;
    uint8_t m_red, m_blue, m_green;
};

class LevelTileSet
{
public:
    LevelTileSet(std::string name,
        std::string filename,
        uint16_t columns,
        uint16_t tilecount,
        uint16_t tilewidth,
        uint16_t tileheight,
        uint16_t spacing,
        uint16_t margin);

    bool getTileClipping(
        uint16_t tile_id, int& x, int& y, int& w, int& h) const;

    std::string get_filename() const;
    sdl_wrap::Texture* get_texture();
    void set_texture(std::shared_ptr<sdl_wrap::Texture> sp_texture);

private:
    std::shared_ptr<sdl_wrap::Texture> m_sp_texture;
    std::string m_name;
    std::string m_filename;
    std::shared_ptr<spdlog::logger> m_sp_logger;
    uint16_t m_columns;
    uint16_t m_tilecount;
    uint16_t m_tilewidth;
    uint16_t m_tileheight;
    uint16_t m_spacing;
    uint16_t m_margin;
};

class Level
{
public:
    Level(uint16_t size_x, uint16_t size_y, float scale);

    ~Level();

    int16_t get(uint16_t x, uint16_t y) const;

    float get_scale(void) const;

    void get_size(uint16_t& x, uint16_t& y) const;

    void get_tile(
        float world_x, float world_y, uint16_t& tile_x, uint16_t& tile_y) const;
    LevelTileSet* get_tileset() const;

    void load(const std::string* initialdata,
        uint16_t size_x,
        uint16_t size_y,
        std::map<std::string, const LevelTile*> tileset);

    void print() const;

    void set_tileset(LevelTileSet* p_tileset);

    void set(uint16_t pos_x, uint16_t pos_y, int16_t tile_id);

private:
    const uint16_t m_size_x, m_size_y;

    // Number of world units per tile
    const float m_scale;

    int16_t* m_p_tiles;

    std::unique_ptr<LevelTileSet> m_up_tileset;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* CORE_LEVEL_HPP */
