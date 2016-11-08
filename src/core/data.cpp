#include "data.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"

#include <iostream>
#include <fstream>
#include <map>

namespace core
{

void factory_component_texture(const Json::Value data, anax::Entity entity)
{
    std::string texture_path =
        data.get("texture_path", "resources/missing.png").asString();
    entity.addComponent<components::TextureComponent>(texture_path);
}

void factory_component_transform(const Json::Value data, anax::Entity entity)
{
    float pos_x = data.get("pos_x", 0.0f).asFloat();
    float pos_y = data.get("pos_y", 0.0f).asFloat();
    float size_x = data.get("size_x", 0.0f).asFloat();
    float size_y = data.get("size_y", 0.0f).asFloat();
    float rotation = data.get("rotation", 0.0f).asFloat();
    bool flip_vert = data.get("flip_vert", false).asBool();
    bool flip_horiz = data.get("flip_horiz", false).asBool();
    entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, size_x, size_y, rotation, flip_vert, flip_horiz);
}

void factory_component_velocity(const Json::Value data, anax::Entity entity)
{
    float velocity_x = data.get("x", 0.0f).asFloat();
    float velocity_y = data.get("y", 0.0f).asFloat();
    entity.addComponent<components::VelocityComponent>(velocity_x, velocity_y);
}

std::map<std::string, void (*)(const Json::Value, anax::Entity)>
    component_factories = {
        {"texture", factory_component_texture},
        {"transform", factory_component_transform},
        {"velocity", factory_component_velocity},
};

DataReader::DataReader(std::string filename) : m_str_filename(filename)
{
    m_sp_logger = logging_get_logger("data");
    Json::Reader reader_json;
    m_sp_logger->info("Loading data from {}", filename);
    std::ifstream config_file(filename, std::ifstream::binary);
    reader_json.parse(config_file, m_json_config);
}

anax::Entity DataReader::makeEntity(std::string entityname, anax::World& world)
{
    auto entity = world.createEntity();
    auto components = m_json_config[entityname]["components"];
    m_sp_logger->info("Components list for entity name {} size {}",
        entityname,
        components.size());
    for (int index = 0; index < components.size(); ++index)
    {
        const std::string type = components[index]["type"].asString();
        m_sp_logger->info("Creating component {}", type);
        component_factories[type](components[index], entity);
    }
    return entity;
}

} // namespace core
