#include "level.hpp"

#include <iostream>
#include <utility>

namespace core
{

int& Level::get_tile(int pos_x, int pos_y, int layer)
{
    return const_cast<int&>(
        const_cast<const Level*>(this)->get_tile(pos_x, pos_y, layer));
}

const int& Level::get_tile(int pos_x, int pos_y, int layer) const
{
    const int* p_ret_val = nullptr;
    if (pos_x >= m_size_x || pos_x < 0 || pos_y >= m_size_y || pos_y < 0)
    {
        p_ret_val = &m_default_tile;
    }
    else
    {
        p_ret_val = &(m_p_tiles[pos_x + (pos_y * m_size_x) +
            (layer * m_size_y * m_size_x)]);
    }
    return *p_ret_val;
}

bool Level::get_collision(int pos_x, int pos_y) const
{
    const auto tile = get_tile(pos_x, pos_y, m_collision_layer);

    return tile > 0;
}
int Level::get_collision_layer() const
{
    return m_collision_layer;
}

Level::Level(int size_x, int size_y, int layers, float scale, int default_tile)
    : m_size_x(size_x),
      m_size_y(size_y),
      m_layers(layers),
      m_scale(scale),
      m_default_tile(default_tile),
      m_collision_layer(-1),
      m_sp_logger(logging_get_logger("level"))
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

void Level::get_tile_coords(
    float world_x, float world_y, int& tile_x, int& tile_y) const
{
    tile_x = world_x / m_scale;
    tile_y = world_y / m_scale;
}
void Level::get_world_coords(
    int tile_x, int tile_y, float& world_x, float& world_y) const
{
    world_x = tile_x * m_scale;
    world_y = tile_y * m_scale;
}

void Level::get_tileset(
    int tile, LevelTileSet*& p_tileset, int& tileset_tile) const
{
    int result_index = -1;
    for (unsigned int index = 0; index < m_vec_firsttile.size(); index++)
    {
        if (tile < m_vec_firsttile[index])
        {
            result_index = index - 1;
            break;
        }
        else if (tile == m_vec_firsttile[index])
        {
            result_index = index;
            break;
        }
        else if (index == m_vec_firsttile.size() - 1)
        {
            result_index = index;
            break;
        }
    }
    if (result_index != -1)
    {
        p_tileset = m_vec_tileset.at(result_index).get();
        tileset_tile = tile - m_vec_firsttile[result_index];
    }
    else
    {
        m_sp_logger->error("no match found for tile {}", tile);
        p_tileset = nullptr;
    }
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
                auto currtile = get_tile(pos_x, pos_y, layer);
                std::cout << '[' << currtile << ']';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Level::set(int pos_x, int pos_y, int layer, int tile_id)
{
    auto& tile = get_tile(pos_x, pos_y, layer);
    tile = tile_id;
}

void Level::set_collision_layer(int collision_layer)
{
    m_collision_layer = collision_layer;
}

void Level::add_tileset(int first_tile, LevelTileSet* p_tileset)
{
    // TODO(Keegan): Place these in order
    m_vec_firsttile.emplace_back(first_tile);
    m_vec_tileset.emplace_back(p_tileset);
}

LevelTileSet::LevelTileSet(std::string name,
    std::string filename,
    unsigned int columns,
    unsigned int tilecount,
    unsigned int tilewidth,
    unsigned int tileheight,
    unsigned int spacing,
    unsigned int margin)
    : m_name(std::move(name)),
      m_filename(std::move(filename)),
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
    int tilecoord_x = (tile_id) % m_columns;
    int tilecoord_y = (tile_id) / m_columns;

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
