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

const std::string component_name_camera{"camera"};
const std::string component_name_player{"player"};
const std::string component_name_texture{"texture"};
const std::string component_name_transform{"transform"};
const std::string component_name_velocity{"velocity"};

void DataReader::check_required_component_property(
    const Json::Value data, std::string component, std::string property)
{
    if (!data.isMember(property))
    {
        m_sp_logger->error(
            "JSON data component {} missing property {}", component, property);
        throw ExceptionParseFailure(
            m_str_filename, "JSON Data component missing property");
    }
}

void DataReader::factory_component_player(
    const Json::Value data, anax::Entity entity)
{
    entity.addComponent<components::PlayerComponent>();
}

void DataReader::factory_component_camera(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_zoom{"zoom"};
    const std::string prop_target{"target"};
    check_required_component_property(data, component_name_camera, prop_zoom);
    check_required_component_property(data, component_name_camera, prop_target);
    float zoom = data.get(prop_zoom, 1.0f).asFloat();
    std::string target = data.get(prop_target, "player").asString();
    auto player = m_map_entities[target];
    entity.addComponent<components::CameraComponent>(player, zoom);
}

void DataReader::factory_component_texture(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_texture_path{"texture_path"};
    check_required_component_property(
        data, component_name_texture, prop_texture_path);
    std::string texture_path =
        data.get(prop_texture_path, "resources/missing.png").asString();
    entity.addComponent<components::TextureComponent>(texture_path);
}

void DataReader::factory_component_transform(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_pos_x{"pos_x"};
    const std::string prop_pos_y{"pos_y"};
    const std::string prop_size_x{"size_x"};
    const std::string prop_size_y{"size_y"};
    const std::string prop_rotation{"rotation"};
    const std::string prop_flip_horiz{"flip_horiz"};
    const std::string prop_flip_vert{"flip_vert"};
    check_required_component_property(
        data, component_name_transform, prop_pos_x);
    check_required_component_property(
        data, component_name_transform, prop_pos_y);
    check_required_component_property(
        data, component_name_transform, prop_size_x);
    check_required_component_property(
        data, component_name_transform, prop_size_y);
    check_required_component_property(
        data, component_name_transform, prop_rotation);
    check_required_component_property(
        data, component_name_transform, prop_flip_vert);
    check_required_component_property(
        data, component_name_transform, prop_flip_horiz);
    float pos_x = data.get(prop_pos_x, 0.0f).asFloat();
    float pos_y = data.get(prop_pos_y, 0.0f).asFloat();
    float size_x = data.get(prop_size_x, 0.0f).asFloat();
    float size_y = data.get(prop_size_y, 0.0f).asFloat();
    float rotation = data.get(prop_rotation, 0.0f).asFloat();
    bool flip_vert = data.get(prop_flip_vert, false).asBool();
    bool flip_horiz = data.get(prop_flip_horiz, false).asBool();
    entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, size_x, size_y, rotation, flip_vert, flip_horiz);
}

void DataReader::factory_component_velocity(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_vel_x{"vel_x"};
    const std::string prop_vel_y{"vel_y"};
    check_required_component_property(
        data, component_name_velocity, prop_vel_x);
    check_required_component_property(
        data, component_name_velocity, prop_vel_y);
    float velocity_x = data.get(prop_vel_x, 0.0f).asFloat();
    float velocity_y = data.get(prop_vel_y, 0.0f).asFloat();
    entity.addComponent<components::VelocityComponent>(velocity_x, velocity_y);
}

DataReader::DataReader(std::string filename) : m_str_filename(filename)
{
    m_sp_logger = logging_get_logger("data");

    component_factories.insert(std::make_pair(
        component_name_camera, &DataReader::factory_component_camera));
    component_factories.insert(std::make_pair(
        component_name_player, &DataReader::factory_component_player));
    component_factories.insert(std::make_pair(
        component_name_texture, &DataReader::factory_component_texture));
    component_factories.insert(std::make_pair(
        component_name_transform, &DataReader::factory_component_transform));
    component_factories.insert(std::make_pair(
        component_name_velocity, &DataReader::factory_component_velocity));

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
