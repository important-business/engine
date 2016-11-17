#include "components/camera.hpp"
#include "components/collision.hpp"
#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "data.hpp"
#include "exception.hpp"

#include <fstream>
#include <iostream>
#include <map>

namespace core
{

const std::string component_name_camera{"camera"};
const std::string component_name_collision{"collision"};
const std::string component_name_player{"player"};
const std::string component_name_texture{"texture"};
const std::string component_name_transform{"transform"};
const std::string component_name_velocity{"velocity"};

void DataReader::factory_component_player(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_move_accel{"move_accel"};
    const std::string prop_idle_accel{"idle_accel"};
    const std::string prop_top_speed{"top_speed"};

    check_required_component_property(
        data, component_name_player, prop_idle_accel);
    float idle_accel = data[prop_idle_accel].asFloat();
    check_required_component_property(
        data, component_name_player, prop_move_accel);
    float move_accel = data[prop_move_accel].asFloat();
    check_required_component_property(
        data, component_name_player, prop_top_speed);
    float top_speed = data[prop_top_speed].asFloat();
    entity.addComponent<components::PlayerComponent>(
        idle_accel, move_accel, top_speed);
}

void DataReader::factory_component_camera(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_zoom{"zoom"};
    const std::string prop_target{"target"};

    float zoom = data.get(prop_zoom, 1.0f).asFloat();

    check_required_component_property(data, component_name_camera, prop_target);
    std::string target = data[prop_target].asString();

    auto player = m_map_entities[target];

    entity.addComponent<components::CameraComponent>(player, zoom);
}

void DataReader::factory_component_collision(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_x{"x"};
    const std::string prop_y{"y"};
    const std::string prop_h{"h"};
    const std::string prop_w{"w"};
    const std::string prop_causeevents{"cancauseevents"};

    int x = data.get(prop_x, 0).asInt();
    int y = data.get(prop_y, 0).asInt();

    check_required_component_property(data, component_name_collision, prop_w);
    check_required_component_property(data, component_name_collision, prop_h);
    check_required_component_property(
        data, component_name_collision, prop_causeevents);
    int h = data[prop_h].asInt();
    int w = data[prop_w].asInt();
    bool cancauseevents = data[prop_causeevents].asBool();

    entity.addComponent<components::Collision>(x, y, h, w, cancauseevents);
}

void DataReader::factory_component_texture(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_texture_path{"texture_path"};

    check_required_component_property(
        data, component_name_texture, prop_texture_path);
    std::string texture_path = data[prop_texture_path].asString();

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
    float pos_x = data[prop_pos_x].asFloat();

    check_required_component_property(
        data, component_name_transform, prop_pos_y);
    float pos_y = data[prop_pos_y].asFloat();

    check_required_component_property(
        data, component_name_transform, prop_size_x);
    float size_x = data[prop_size_x].asFloat();

    check_required_component_property(
        data, component_name_transform, prop_size_y);
    float size_y = data[prop_size_y].asFloat();

    check_required_component_property(
        data, component_name_transform, prop_rotation);
    float rotation = data[prop_rotation].asFloat();

    check_required_component_property(
        data, component_name_transform, prop_flip_vert);
    bool flip_vert = data[prop_flip_vert].asBool();

    check_required_component_property(
        data, component_name_transform, prop_flip_horiz);
    bool flip_horiz = data[prop_flip_horiz].asBool();

    entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, size_x, size_y, rotation, flip_vert, flip_horiz);
}

void DataReader::factory_component_velocity(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_inertia{"inertia"};
    const std::string prop_friction{"friction"};
    const std::string prop_force_x{"force_x"};
    const std::string prop_force_y{"force_y"};
    const float prop_force_x_default = 0.0f;
    const float prop_force_y_default = 0.0f;
    const std::string prop_vel_x{"vel_x"};
    const std::string prop_vel_y{"vel_y"};
    const float prop_vel_x_default = 0.0f;
    const float prop_vel_y_default = 0.0f;

    float velocity_x = data.get(prop_vel_x, prop_vel_x_default).asFloat();
    float velocity_y = data.get(prop_vel_y, prop_vel_y_default).asFloat();
    float force_x = data.get(prop_force_x, prop_force_x_default).asFloat();
    float force_y = data.get(prop_force_y, prop_force_y_default).asFloat();
    check_required_component_property(
        data, component_name_velocity, prop_inertia);
    float inertia = data[prop_inertia].asFloat();

    check_required_component_property(
        data, component_name_velocity, prop_friction);
    float friction = data[prop_friction].asFloat();

    entity.addComponent<components::VelocityComponent>(
        inertia, friction, velocity_x, velocity_y, force_x, force_y);
}

DataReader::DataReader(std::string filename) : JsonFileReader(filename)
{
    m_sp_logger = logging_get_logger("data");

    component_factories.insert(std::make_pair(
        component_name_camera, &DataReader::factory_component_camera));
    component_factories.insert(std::make_pair(
        component_name_collision, &DataReader::factory_component_collision));
    component_factories.insert(std::make_pair(
        component_name_player, &DataReader::factory_component_player));
    component_factories.insert(std::make_pair(
        component_name_texture, &DataReader::factory_component_texture));
    component_factories.insert(std::make_pair(
        component_name_transform, &DataReader::factory_component_transform));
    component_factories.insert(std::make_pair(
        component_name_velocity, &DataReader::factory_component_velocity));
}

anax::Entity DataReader::makeEntity(std::string entityname, anax::World& world)
{
    const std::string prop_name_components{"components"};
    const std::string prop_name_template{"template"};

    if (!m_json_data.isMember(entityname))
    {
        m_sp_logger->error("JSON data {} missing referenced entity {}",
            m_str_description,
            entityname);
        throw ExceptionParseFailure(
            m_str_description, "Missing referenced entity");
    }

    auto entity = world.createEntity();

    Json::Value entity_data;
    if (m_json_data[entityname].isMember(prop_name_template))
    {
        std::string templatename =
            m_json_data[entityname][prop_name_template].asString();
        m_sp_logger->info(
            "Using template {} for entity {}", templatename, entityname);
        entity_data = merge_values(
            m_map_references[templatename], m_json_data[entityname]);
    }
    else
    {
        entity_data = m_json_data[entityname];
    }

    if (!entity_data.isMember(prop_name_components))
    {
        m_sp_logger->error("JSON data {} entity {} missing {}",
            m_str_description,
            entityname,
            prop_name_components);
        throw ExceptionParseFailure(
            m_str_description, "JSON data entity missing components");
    }

    auto components = entity_data[prop_name_components];

    m_sp_logger->info("Components list for entity name {} size {}",
        entityname,
        components.size());

    Json::Value::Members member_names = components.getMemberNames();
    for (auto type : member_names)
    {
        m_sp_logger->info("Creating component {}", type);

        factory_method fp = component_factories[type];
        (this->*fp)(components[type], entity);
    }

    entity.activate();
    return entity;
}

void DataReader::makeEntities(anax::World& world)
{
    const std::string object_name_world{"world"};
    const std::string prop_name_entities{"entities"};

    scan_references(m_json_data);

    if (!m_json_data.isMember(object_name_world))
    {
        m_sp_logger->error(
            "JSON data {} missing {}", m_str_description, object_name_world);
        throw ExceptionParseFailure(
            m_str_description, "JSON Data missing world");
    }
    if (!m_json_data[object_name_world].isMember(prop_name_entities))
    {
        m_sp_logger->error("JSON data {} world missing {}",
            m_str_description,
            prop_name_entities);
        throw ExceptionParseFailure(
            m_str_description, "JSON Data world missing entities");
    }

    auto entities = m_json_data[object_name_world][prop_name_entities];
    m_sp_logger->info("Entities list for world size {}", entities.size());
    for (auto value : entities)
    {
        std::string name = value.asString();
        auto entity = makeEntity(name, world);
        m_map_entities.insert({name, entity});
    }
}

} // namespace core
