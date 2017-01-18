#ifndef CORE_LEVEL_HPP
#define CORE_LEVEL_HPP

#include "core/logging.hpp"
#include "sdl_wrap/wrap.hpp"

#include <cstdint>
#include <memory>
namespace core
{

class LevelTileSet
{
public:
    LevelTileSet(std::string name,
        std::string filename,
        unsigned int columns,
        unsigned int tilecount,
        unsigned int tilewidth,
        unsigned int tileheight,
        unsigned int spacing,
        unsigned int margin);

    bool getTileClipping(
        int tile_id, int& x, int& y, int& w, int& h) const;

    std::string get_filename() const;
    sdl_wrap::Texture* get_texture();
    void set_texture(std::shared_ptr<sdl_wrap::Texture> sp_texture);

    unsigned int get_tilecount() const;

private:
    std::shared_ptr<sdl_wrap::Texture> m_sp_texture;
    std::string m_name;
    std::string m_filename;
    std::shared_ptr<spdlog::logger> m_sp_logger;
    unsigned int m_columns, m_tilecount;
    unsigned int m_tilewidth ,m_tileheight;
    unsigned int m_spacing, m_margin;
};

class Level
{
public:
    Level(int size_x, int size_y, int layers, float scale, int default_tile);

    ~Level();

    float get_scale() const;

    void get_size(int& x, int& y, int& layers) const;

    int& get_tile(int pos_x, int pos_y, int layer);
    const int& get_tile(
        int pos_x, int pos_y, int layer) const;

    void get_tile_coords(
        float world_x, float world_y, int& tile_x, int& tile_y) const;

    void get_world_coords(
        int tile_x, int tile_y, float& world_x, float& world_y) const;

    void get_tileset(int tile, LevelTileSet* &p_tileset, int &tileset_tile) const;

    void print() const;

    void add_tileset(int first_tile, LevelTileSet* p_tileset);

    void set(int pos_x, int pos_y, int layer, int tile_id);

private:
    const int m_size_x, m_size_y, m_layers;

    // Number of world units per tile
    const float m_scale;

    int* m_p_tiles;

    int m_default_tile;

    std::vector<int> m_vec_firsttile;
    std::vector<std::unique_ptr<LevelTileSet>> m_vec_tileset;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* CORE_LEVEL_HPP */
