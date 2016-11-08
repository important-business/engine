#include "data.hpp"
#include "exception.hpp"
#include "components/camera.hpp"
#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"

#include <iostream>
#include <fstream>
#include <map>

namespace core
{

void DataReader::factory_component_player(
    const Json::Value data, anax::Entity entity)
{
    entity.addComponent<components::PlayerComponent>();
}

void DataReader::factory_component_camera(
    const Json::Value data, anax::Entity entity)
{
    float zoom = data.get("zoom", 1.0f).asFloat();
    std::string target = data.get("target", "player").asString();
    auto player = m_map_entities[target];
    entity.addComponent<components::CameraComponent>(player, zoom);
}

void DataReader::factory_component_texture(
    const Json::Value data, anax::Entity entity)
{
    std::string texture_path =
        data.get("texture_path", "resources/missing.png").asString();
    entity.addComponent<components::TextureComponent>(texture_path);
}

void DataReader::factory_component_transform(
    const Json::Value data, anax::Entity entity)
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

void DataReader::factory_component_velocity(
    const Json::Value data, anax::Entity entity)
{
    float velocity_x = data.get("x", 0.0f).asFloat();
    float velocity_y = data.get("y", 0.0f).asFloat();
    entity.addComponent<components::VelocityComponent>(velocity_x, velocity_y);
}

DataReader::DataReader(std::string filename) : m_str_filename(filename)
{
    component_factories.insert(
        std::make_pair("camera", &DataReader::factory_component_camera));
    component_factories.insert(
        std::make_pair("player", &DataReader::factory_component_player));
    component_factories.insert(
        std::make_pair("texture", &DataReader::factory_component_texture));
    component_factories.insert(
        std::make_pair("transform", &DataReader::factory_component_transform));
    component_factories.insert(
        std::make_pair("velocity", &DataReader::factory_component_velocity));
    m_sp_logger = logging_get_logger("data");
    Json::Reader reader_json;
    m_sp_logger->info("Loading data from {}", filename);
    std::ifstream config_file(filename, std::ifstream::binary);
    if (!reader_json.parse(config_file, m_json_config))
    {
        m_sp_logger->error(
            "Failed to parse JSON file {}:", filename, "JSON format error");
        m_sp_logger->error(reader_json.getFormattedErrorMessages());
        throw ExceptionParseFailure(
            m_str_filename, std::string("JSON format error"));
    }
}

anax::Entity DataReader::makeEntity(std::string entityname, anax::World& world)
{
    if (!m_json_config.isMember(entityname))
    {
        m_sp_logger->error("JSON data {} missing referenced entity {}",
            m_str_filename,
            entityname);
        throw ExceptionParseFailure(
            m_str_filename, "Missing referenced entity");
    }

    auto entity = world.createEntity();

    if (!m_json_config[entityname].isMember("components"))
    {
        m_sp_logger->error("JSON data {} entity {} missing components",
            m_str_filename,
            entityname);
        throw ExceptionParseFailure(
            m_str_filename, "JSON data entity missing components");
    }

    auto components = m_json_config[entityname]["components"];

    m_sp_logger->info("Components list for entity name {} size {}",
        entityname,
        components.size());

    for (auto it = components.begin(); it != components.end(); ++it)
    {
        if (!it->isMember("type"))
        {
            m_sp_logger->error(
                "JSON data {} entity {} has component with missing type",
                m_str_filename,
                entityname);
            throw ExceptionParseFailure(
                m_str_filename, "component missing type");
        }

        const std::string type = (*it)["type"].asString();

        m_sp_logger->info("Creating component {}", type);

        factory_method fp = component_factories[type];
        (this->*fp)(*it, entity);
    }

    entity.activate();
    return entity;
}

void DataReader::makeEntities(anax::World& world)
{
    if (!m_json_config.isMember("world"))
    {
        m_sp_logger->error("JSON data {} missing world", m_str_filename);
        throw ExceptionParseFailure(m_str_filename, "JSON Data missing world");
    }
    if (!m_json_config["world"].isMember("entities"))
    {
        m_sp_logger->error(
            "JSON data {} world missing entities", m_str_filename);
        throw ExceptionParseFailure(
            m_str_filename, "JSON Data world missing entities");
    }

    auto entities = m_json_config["world"]["entities"];
    m_sp_logger->info("Entities list for world size {}", entities.size());
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        std::string name = (*it).asString();
        auto entity = makeEntity(name, world);
        m_map_entities.insert({name, entity});
    }
}

} // namespace core
