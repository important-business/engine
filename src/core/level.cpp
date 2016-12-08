#include "level.hpp"
#include <iostream>
namespace core
{

int Level::get(int x, int y) const
{
    if ((x > m_size_x) || (y > m_size_y))
    {
        return -1;
    }
    int result = -1;
    for (auto cur_layer = m_layers - 1; cur_layer >= 0; --cur_layer)
    {
        result = get_raw(x, y, cur_layer);
        if (result > 0)
        {
            break;
        }
    }
    return result;
}

int& Level::get_raw(int pos_x, int pos_y, int layer)
{
    return const_cast<int&>(
        const_cast<const Level*>(this)->get_raw(pos_x, pos_y, layer));
}

const int& Level::get_raw(int pos_x, int pos_y, int layer) const
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

Level::Level(int size_x, int size_y, int layers, float scale, int default_tile)
    : m_size_x(size_x),
      m_size_y(size_y),
      m_layers(layers),
      m_scale(scale),
      m_default_tile(default_tile)
{
    m_p_tiles = new int[size_x * size_y * layers];
    for (int layer = 0; layer < layers; layer++)
    {
        for (int pos_x = 0; pos_x < size_x; pos_x++)
        {
            for (int pos_y = 0; pos_y < size_y; pos_y++)
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

void Level::get_size(int& x, int& y, int& layers) const
{
    x = m_size_x;
    y = m_size_y;
    layers = m_layers;
}

void Level::get_tile(
    float world_x, float world_y, int& tile_x, int& tile_y) const
{
    // TODO(Keegan): Don't think it's actually quite this simple
    tile_x = world_x / m_scale;
    tile_y = world_y / m_scale;
}

LevelTileSet* Level::get_tileset() const
{
    return (m_up_tileset.get());
}

unsigned int LevelTileSet::get_tilecount() const
{
    return m_tilecount;
}
void Level::print() const
{
    for (int layer = 0; layer < m_layers; layer++)
    {
        std::cout << "layer " << layer << std::endl;
        for (int pos_y = 0; pos_y < m_size_y; pos_y++)
        {
            for (int pos_x = 0; pos_x < m_size_x; pos_x++)
            {
                auto currtile = get(pos_x, pos_y);
                std::cout << '[' << currtile << ']';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Level::set(int pos_x, int pos_y, int layer, int tile_id)
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
    unsigned int columns,
    unsigned int tilecount,
    unsigned int tilewidth,
    unsigned int tileheight,
    unsigned int spacing,
    unsigned int margin)
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
    int tile_id, int& x, int& y, int& w, int& h) const
{
    int tilecoord_x = (tile_id - 1) % m_columns;
    int tilecoord_y = (tile_id - 1) / m_columns;

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
