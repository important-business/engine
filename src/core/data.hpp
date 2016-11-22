#ifndef CORE_DATA_HPP
#define CORE_DATA_HPP

#include "json.hpp"
#include "level.hpp"
#include "logging.hpp"
#include "core/level.hpp"
#include <string>
#include <anax/anax.hpp>

namespace core
{

class DataReader : public JsonFileReader
{
public:
    typedef void (DataReader::*factory_method)(
        const Json::Value data, anax::Entity);

    DataReader(std::string filename);
    anax::Entity makeEntity(std::string entityname, anax::World& world);
    void makeEntities(anax::World& world);
    std::map<std::string, DataReader::factory_method> component_factories;

private:
    void factory_component_player(const Json::Value data, anax::Entity entity);
    void factory_component_camera(const Json::Value data, anax::Entity entity);
    void factory_component_collision(
        const Json::Value data, anax::Entity entity);
    void factory_component_texture(const Json::Value data, anax::Entity entity);
    void factory_component_transform(
        const Json::Value data, anax::Entity entity);
    void factory_component_velocity(
        const Json::Value data, anax::Entity entity);

    std::map<std::string, anax::Entity> m_map_entities;
};

class LevelReader : public JsonFileReader
{
public:
    void build_level(std::unique_ptr<Level>& up_level);
    LevelTileSet* load_tileset(std::string filename);
    LevelReader(std::string filename);

private:
    Json::Value m_json_tileset;
};

} // namespace core
#endif /* CORE_DATA_HPP */
