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

int32_t Level::get(int32_t x, int32_t y) const
{
    if ((x > m_size_x) || (y > m_size_y))
    {
        return -1;
    }
    int16_t result = -1;
    for (int16_t cur_layer = m_layers - 1; cur_layer >= 0; --cur_layer)
    {
        result = get_raw(x, y, cur_layer);
        if (result > 0)
        {
            break;
        }
    }
    return result;
}

int32_t& Level::get_raw(int32_t pos_x, int32_t pos_y, int32_t layer)
{
    return const_cast<int32_t&>(
        const_cast<const Level*>(this)->get_raw(pos_x, pos_y, layer));
}

const int32_t& Level::get_raw(int32_t pos_x, int32_t pos_y, int32_t layer) const
{
    if (pos_x > m_size_x || pos_x < 0 || pos_y > m_size_y || pos_y < 0)
    {
        return m_default_tile;
    }
    else
    {
        return m_p_tiles[pos_x + (pos_y * m_size_x) +
            (layer * m_size_y * m_size_x)];
    }
}

Level::Level(int32_t size_x,
    int32_t size_y,
    int32_t layers,
    float scale,
    int32_t default_tile)
    : m_size_x(size_x),
      m_size_y(size_y),
      m_layers(layers),
      m_scale(scale),
      m_default_tile(default_tile)
{
    m_p_tiles = new int32_t[size_x * size_y * layers];
    for (int32_t layer = 0; layer < layers; layer++)
    {
        for (int32_t pos_x = 0; pos_x < size_x; pos_x++)
        {
            for (int32_t pos_y = 0; pos_y < size_y; pos_y++)
            {
                set(pos_x, pos_y, layer, -1);
            }
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

void Level::get_size(int32_t& x, int32_t& y, int32_t& layers) const
{
    x = m_size_x;
    y = m_size_y;
    layers = m_layers;
}

void Level::get_tile(
    float world_x, float world_y, int32_t& tile_x, int32_t& tile_y) const
{
    // TODO(Keegan): Don't think it's actually quite this simple
    tile_x = world_x / m_scale;
    tile_y = world_y / m_scale;
}

LevelTileSet* Level::get_tileset() const
{
    return (m_up_tileset.get());
}

uint16_t LevelTileSet::get_tilecount() const
{
    return m_tilecount;
}
void Level::print() const
{
    for (int32_t layer = 0; layer < m_layers; layer++)
    {
        std::cout << "layer " << layer << std::endl;
        for (int32_t pos_y = 0; pos_y < m_size_y; pos_y++)
        {
            for (int32_t pos_x = 0; pos_x < m_size_x; pos_x++)
            {
                auto currtile = get(pos_x, pos_y);
                std::cout << '[' << currtile << ']';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Level::set(int32_t pos_x, int32_t pos_y, int32_t layer, int16_t tile_id)
{
    auto& tile = get_raw(pos_x, pos_y, layer);
    tile = tile_id;
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
