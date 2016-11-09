#ifndef DATA_HPP
#define DATA_HPP

#include "logging.hpp"
#include <json/json.h>
#include <string>
#include <anax/anax.hpp>

namespace core
{

class DataReader
{
public:
    typedef void (DataReader::*factory_method)(
        const Json::Value data, anax::Entity);

    DataReader(std::string filename);
    anax::Entity makeEntity(std::string entityname, anax::World& world);
    void makeEntities(anax::World& world);
    std::map<std::string, DataReader::factory_method> component_factories;

private:
    void check_required_component_property(
        const Json::Value data, std::string component, std::string property);
    void factory_component_player(const Json::Value data, anax::Entity entity);
    void factory_component_camera(const Json::Value data, anax::Entity entity);
    void factory_component_texture(const Json::Value data, anax::Entity entity);
    void factory_component_transform(
        const Json::Value data, anax::Entity entity);
    void factory_component_velocity(
        const Json::Value data, anax::Entity entity);
    void scan_references(Json::Value data);
    Json::Value merge_values(Json::Value data, Json::Value overlay);
    const std::string m_str_filename;
    Json::Value m_json_config;
    std::shared_ptr<spdlog::logger> m_sp_logger;
    std::map<std::string, anax::Entity> m_map_entities;
    std::map<std::string, Json::Value> m_map_references;
};

} // namespace core
#endif /* DATA_HPP */
