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

int16_t Level::get(uint16_t x, uint16_t y) const
{
    if ((x > m_size_x) || (y > m_size_y))
    {
        return -1;
    }
    return m_p_tiles[x + y * m_size_x];
}

Level::Level(uint16_t size_x, uint16_t size_y, float scale)
    : m_size_x(size_x), m_size_y(size_y), m_scale(scale)
{
    m_p_tiles = new int16_t[size_x * size_y];
    for (uint16_t pos_x = 0; pos_x < size_x; pos_x++)
    {
        for (uint16_t pos_y = 0; pos_y < size_y; pos_y++)
        {
            set(pos_x, pos_y, -1);
        }
    }
    m_sp_logger = logging_get_logger("level");
}

Level::~Level()
{
    delete[] m_p_tiles;
    m_p_tiles = nullptr;
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

LevelTileSet* Level::get_tileset() const
{
    return (m_up_tileset.get());
}

/* void Level::load(const std::string* initialdata, */
/*     uint16_t size_x, */
/*     uint16_t size_y, */
/*     std::map<std::string, const LevelTile*> tileset) */
/* { */
/*     for (uint16_t pos_x = 0; pos_x < size_x; pos_x++) */
/*     { */
/*         for (uint16_t pos_y = 0; pos_y < size_y; pos_y++) */
/*         { */
/*             auto tiledata = initialdata[pos_x + pos_y * size_x]; */
/*             m_sp_logger->info("Loading {} into {},{}", tiledata, pos_x,
 * pos_y); */
/*             // TODO: Check for and handle missing tile type */
/*             set(pos_x, pos_y, tileset[tiledata]); */
/*         } */
/*     } */
/* } */

void Level::print() const
{
    for (uint16_t pos_y = 0; pos_y < m_size_y; pos_y++)
    {
        for (uint16_t pos_x = 0; pos_x < m_size_x; pos_x++)
        {
            auto currtile = get(pos_x, pos_y);
            std::cout << '[' << currtile << ']';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Level::set(uint16_t pos_x, uint16_t pos_y, int16_t tile_id)
{
    m_p_tiles[pos_x + pos_y * m_size_y] = tile_id;
}

void Level::set_tileset(LevelTileSet* p_tileset)
{
    m_up_tileset = std::unique_ptr<LevelTileSet>(p_tileset);
}

LevelTileSet::LevelTileSet(std::string name,
    std::string filename,
    uint16_t columns,
    uint16_t tilecount,
    uint16_t tilewidth,
    uint16_t tileheight,
    uint16_t spacing,
    uint16_t margin)
    : m_name(name),
      m_filename(filename),
      m_columns(columns),
      m_tilecount(tilecount),
      m_tilewidth(tilewidth),
      m_tileheight(tileheight),
      m_spacing(spacing),
      m_margin(margin)
{
    m_sp_logger = logging_get_logger("level");
}

bool LevelTileSet::getTileClipping(
    uint16_t tile_id, int& x, int& y, int& w, int& h) const
{
    int16_t tilecoord_x = (tile_id - 1) % m_columns;
    int16_t tilecoord_y = (tile_id - 1) / m_columns;

    x = m_margin + (m_tilewidth + m_spacing) * tilecoord_x;
    y = m_margin + (m_tileheight + m_spacing) * tilecoord_y;
    w = m_tilewidth;
    h = m_tileheight;

    return false;
}

std::string LevelTileSet::get_filename() const
{
    return m_filename;
}

sdl_wrap::Texture* LevelTileSet::get_texture()
{
    return m_sp_texture.get();
}

void LevelTileSet::set_texture(std::shared_ptr<sdl_wrap::Texture> sp_texture)
{
    m_sp_texture = sp_texture;
}
} // namespace core
