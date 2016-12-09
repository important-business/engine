#include "data.hpp"
#include "components/ai.hpp"
#include "components/camera.hpp"
#include "components/collision.hpp"
#include "components/id.hpp"
#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "exception.hpp"

#include <cassert>
#include <fstream>
#include <map>

namespace core
{

std::string basename(const std::string& pathname)
{
    // TODO(Keegan): Check if works on Windows)
    // TODO(Keegan): Test properly)
    return {std::find_if(pathname.rbegin(),
                pathname.rend(),
                [](char c) { return (c == '/' || c == '\\'); })
                .base(),
        pathname.end()};
}

std::string pathname(const std::string& pathname)
{
    // TODO(Keegan); Check if works on Windows)
    // TODO(Keegan): Test properly)
    return {pathname.begin(),
        std::find_if(pathname.rbegin(), pathname.rend(), [](char c) {
            return (c == '/' || c == '\\');
        }).base()};
}

static std::string base64_decode(const std::string& in)
{

    std::string out;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
    {
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
                [i]] = i;
    }

    int val = 0, valb = -8;
    for (unsigned char c : in)
    {
        if (T[c] == -1)
        {
            break;
        }
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

static std::vector<int32_t> string_decode(const std::string& in)
{
    std::vector<int32_t> result;
    if (in.size() % 4 != 0u)
    {
        // TODO(Keegan): Handle error better)
        assert(false);
    }
    for (auto it = in.begin(); it < in.end(); it += 4)
    {
        auto b1 = *(it);
        auto b2 = *(it + 1);
        auto b3 = *(it + 2);
        auto b4 = *(it + 3);
        int32_t val = b1;
        // TODO(Keegan): This is awfully endian dependent...)
        val |= b2 << 8;
        val |= b3 << 16;
        val |= b4 << 24;
        result.push_back(val);
    }
    return result;
}

void DataReader::factory_component_ai(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_root_node{"root_node"};

    check_required_component_property(
        data, components::AiComponent::name, prop_root_node);
    // TODO(Keegan): use real root node data)
    auto p_root_node = factory_ai_node(data[prop_root_node]);
    entity.addComponent<components::AiComponent>(p_root_node);
}

systems::AiNode* DataReader::factory_ai_node(const Json::Value& data)
{
    const std::string prop_type{"type"};
    const std::string prop_type_node{"node"};
    const std::string prop_type_sequence{"sequence"};
    const std::string prop_type_loop{"loop"};
    const std::string prop_type_selector{"selector"};
    const std::string prop_type_invert{"invert"};
    const std::string prop_type_moveto{"moveto"};
    const std::string prop_type_follow{"follow"};
    const std::string prop_children{"children"};
    const std::string prop_decoratee{"decoratee"};
    const std::string prop_pos_x{"pos_x"};
    const std::string prop_pos_y{"pos_y"};
    const std::string prop_target{"target"};
    const std::string prop_follow_x{"follow_x"};
    const std::string prop_follow_y{"follow_y"};
    const std::string prop_tolerance{"tolerance"};

    const std::string type = data["type"].asString();

    m_sp_logger->info("Found node {}", type);
    systems::AiNode* p_to_return = nullptr;
    if (type.compare(prop_type_node) == 0)
    {
        p_to_return = new systems::AiNode();
    }
    else if (type.compare(prop_type_sequence) == 0)
    {
        auto p_node = new systems::AiNodeSequence();
        for (auto child : data[prop_children])
        {
            m_sp_logger->info("Building child node");
            auto p_child_node = factory_ai_node(child);
            p_node->add_child(p_child_node);
        }
        p_to_return = p_node;
    }
    else if (type.compare(prop_type_selector) == 0)
    {
        auto p_node = new systems::AiNodeSelector();
        for (auto child : data[prop_children])
        {
            m_sp_logger->info("Building child node");
            auto p_child_node = factory_ai_node(child);
            p_node->add_child(p_child_node);
        }
        p_to_return = p_node;
    }
    else if (type.compare(prop_type_loop) == 0)
    {
        auto p_node = new systems::AiNodeLoop();
        for (auto child : data[prop_children])
        {
            m_sp_logger->info("Building child node");
            auto p_child_node = factory_ai_node(child);
            p_node->add_child(p_child_node);
        }
        p_to_return = p_node;
    }
    else if (type.compare(prop_type_invert) == 0)
    {
        auto p_decoratee = factory_ai_node(data[prop_decoratee]);
        auto p_node = new systems::AiNodeDecoratorInvert(p_decoratee);
        p_to_return = p_node;
    }
    else if (type.compare(prop_type_moveto) == 0)
    {
        auto pos_x = data[prop_pos_x].asDouble();
        auto pos_y = data[prop_pos_y].asDouble();
        auto tolerance = data[prop_tolerance].asDouble();
        auto p_node = new systems::AiNodeMoveTo(pos_x, pos_y, tolerance);
        p_to_return = p_node;
    }
    else if (type.compare(prop_type_follow) == 0)
    {
        auto follow_x = data[prop_follow_x].asBool();
        auto follow_y = data[prop_follow_y].asBool();
        auto tolerance = data[prop_tolerance].asDouble();
        check_required_component_property(
            data, components::CameraComponent::name, prop_target);
        std::string target = data[prop_target].asString();

        auto ent_target = m_map_entities[target];

        auto p_node = new systems::AiNodeFollow(
            ent_target, tolerance, follow_x, follow_y);
        p_to_return = p_node;
    }
    return p_to_return;
}
void DataReader::factory_component_player(
    const Json::Value& data, anax::Entity entity)
{
    entity.addComponent<components::PlayerComponent>();
}

void DataReader::factory_component_camera(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_zoom{"zoom"};

    float zoom = data.get(prop_zoom, 1.0f).asFloat();

    entity.addComponent<components::CameraComponent>(zoom);
}

void DataReader::factory_component_collision(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_x{"x"};
    const std::string prop_y{"y"};
    const std::string prop_h{"h"};
    const std::string prop_w{"w"};
    const std::string prop_causeevents{"cancauseevents"};

    int x = data.get(prop_x, 0).asInt();
    int y = data.get(prop_y, 0).asInt();

    check_required_component_property(
        data, components::Collision::name, prop_w);
    check_required_component_property(
        data, components::Collision::name, prop_h);
    check_required_component_property(
        data, components::Collision::name, prop_causeevents);
    int h = data[prop_h].asInt();
    int w = data[prop_w].asInt();
    bool cancauseevents = data[prop_causeevents].asBool();

    entity.addComponent<components::Collision>(x, y, h, w, cancauseevents);
}

void DataReader::factory_component_texture(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_texture_path{"texture_path"};

    check_required_component_property(
        data, components::TextureComponent::name, prop_texture_path);
    std::string texture_path = data[prop_texture_path].asString();

    entity.addComponent<components::TextureComponent>(texture_path);
}

void DataReader::factory_component_transform(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_pos_x{"pos_x"};
    const std::string prop_pos_y{"pos_y"};
    const std::string prop_size_x{"size_x"};
    const std::string prop_size_y{"size_y"};
    const std::string prop_rotation{"rotation"};
    const std::string prop_flip_horiz{"flip_horiz"};
    const std::string prop_flip_vert{"flip_vert"};

    check_required_component_property(
        data, components::TransformComponent::name, prop_pos_x);
    float pos_x = data[prop_pos_x].asFloat();

    check_required_component_property(
        data, components::TransformComponent::name, prop_pos_y);
    float pos_y = data[prop_pos_y].asFloat();

    check_required_component_property(
        data, components::TransformComponent::name, prop_size_x);
    float size_x = data[prop_size_x].asFloat();

    check_required_component_property(
        data, components::TransformComponent::name, prop_size_y);
    float size_y = data[prop_size_y].asFloat();

    check_required_component_property(
        data, components::TransformComponent::name, prop_rotation);
    float rotation = data[prop_rotation].asFloat();

    check_required_component_property(
        data, components::TransformComponent::name, prop_flip_vert);
    bool flip_vert = data[prop_flip_vert].asBool();

    check_required_component_property(
        data, components::TransformComponent::name, prop_flip_horiz);
    bool flip_horiz = data[prop_flip_horiz].asBool();

    entity.addComponent<components::TransformComponent>(
        pos_x, pos_y, size_x, size_y, rotation, flip_vert, flip_horiz);
}

void DataReader::factory_component_trigger(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_msg{"msg"};

    check_required_component_property(
        data, components::Trigger::name, prop_msg);
    std::string msg = data[prop_msg].asString();

    entity.addComponent<components::Trigger>(msg);
}

void DataReader::factory_component_movement(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_accel{"accel"};
    const std::string prop_top_speed{"top_speed"};

    check_required_component_property(
        data, components::MovementComponent::name, prop_accel);
    float accel = data[prop_accel].asFloat();

    check_required_component_property(
        data, components::MovementComponent::name, prop_top_speed);
    float top_speed = data[prop_top_speed].asFloat();

    entity.addComponent<components::MovementComponent>(accel, top_speed);
}

void DataReader::factory_component_physics(
    const Json::Value& data, anax::Entity entity)
{
    const std::string prop_mass{"mass"};
    const std::string prop_friction{"friction"};
    const std::string prop_force_x{"force_x"};
    const std::string prop_force_y{"force_y"};
    const float prop_force_x_default = 0.0f;
    const float prop_force_y_default = 0.0f;
    const std::string prop_vel_x{"vel_x"};
    const std::string prop_vel_y{"vel_y"};
    const float prop_vel_x_default = 0.0f;
    const float prop_vel_y_default = 0.0f;
    const std::string prop_restitution{"restitution"};

    float velocity_x = data.get(prop_vel_x, prop_vel_x_default).asFloat();
    float velocity_y = data.get(prop_vel_y, prop_vel_y_default).asFloat();
    float force_x = data.get(prop_force_x, prop_force_x_default).asFloat();
    float force_y = data.get(prop_force_y, prop_force_y_default).asFloat();
    check_required_component_property(
        data, components::PhysicsComponent::name, prop_mass);
    float mass = data[prop_mass].asFloat();

    check_required_component_property(
        data, components::PhysicsComponent::name, prop_friction);
    float friction = data[prop_friction].asFloat();

    check_required_component_property(
        data, components::PhysicsComponent::name, prop_restitution);
    float restitution = data[prop_restitution].asFloat();

    entity.addComponent<components::PhysicsComponent>(
        mass, friction, restitution, velocity_x, velocity_y, force_x, force_y);
}

DataReader::DataReader(std::string filename) : JsonFileReader(filename)
{
    m_sp_logger = logging_get_logger("data");

    component_factories.insert(std::make_pair(
        components::AiComponent::name, &DataReader::factory_component_ai));
    component_factories.insert(std::make_pair(components::CameraComponent::name,
        &DataReader::factory_component_camera));
    component_factories.insert(std::make_pair(
        components::Collision::name, &DataReader::factory_component_collision));
    component_factories.insert(std::make_pair(components::PlayerComponent::name,
        &DataReader::factory_component_player));
    component_factories.insert(
        std::make_pair(components::TextureComponent::name,
            &DataReader::factory_component_texture));
    component_factories.insert(
        std::make_pair(components::TransformComponent::name,
            &DataReader::factory_component_transform));
    component_factories.insert(std::make_pair(
        components::Trigger::name, &DataReader::factory_component_trigger));
    component_factories.insert(
        std::make_pair(components::PhysicsComponent::name,
            &DataReader::factory_component_physics));
    component_factories.insert(
        std::make_pair(components::MovementComponent::name,
            &DataReader::factory_component_movement));
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
    entity.addComponent<components::IdComponent>(entityname);
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

LevelReader::LevelReader(std::string filename) : JsonFileReader(filename)
{
    m_sp_logger = logging_get_logger("data");
}

void LevelReader::build_level(std::unique_ptr<Level>& up_level)
{
    unsigned int size_x = m_json_data["width"].asInt();
    unsigned int size_y = m_json_data["height"].asInt();
    unsigned int layer_count = m_json_data["layers"].size();
    float tileheight = m_json_data["tileheight"].asFloat();
    float scale = m_json_data["properties"].get("scale", 1.0).asFloat();
    int default_tile = m_json_data["properties"].get("defaulttile", 1).asInt();

    up_level = std::make_unique<Level>(
        size_x, size_y, layer_count, tileheight * scale, default_tile);
    auto p_level = up_level.get();

    // TODO(Keegan): Use tilewidth as well
    auto tilesets = m_json_data["tilesets"];
    std::string cur_path = pathname(m_str_description);
    std::string tileset_source;
    for (auto& tileset : tilesets)
    {
        int firstgid = tileset["firstgid"].asInt();
        tileset_source = cur_path + tileset["source"].asString();
        m_sp_logger->info(
            "Found a tileset gid {} source {}", firstgid, tileset_source);
    }
    // Only load last tileset for now
    auto p_tileset = load_tileset(tileset_source);
    p_level->set_tileset(p_tileset);

    auto layers = m_json_data["layers"];
    for (unsigned int layerindex = 0; layerindex < layers.size(); ++layerindex)
    {
        auto& val = layers[layerindex];
        int height = val["height"].asInt();
        int width = val["width"].asInt();
        float opacity = val["opacity"].asFloat();
        std::string name = val["name"].asString();
        m_sp_logger->info(
            "found a layer named {} width {} height {} opacity {}",
            name,
            width,
            height,
            opacity);
        auto data = val["data"];
        // TODO(Keegan): check encoding, don't just assume)
        auto decoded = string_decode(base64_decode(data.asString()));
        for (unsigned int index = 0; index < decoded.size(); ++index)
        {
            int tile_x = index % width;
            int tile_y = index / width;
            int tilegid = decoded[index];
            p_level->set(tile_x, tile_y, layerindex, tilegid);
        }
    }

    return;
}

LevelTileSet* LevelReader::load_tileset(std::string filename)
{
    Json::Reader reader_json;
    m_sp_logger->info("Loading data from {}", filename);
    std::ifstream config_file(filename, std::ifstream::binary);
    if (!reader_json.parse(config_file, m_json_tileset))
    {
        m_sp_logger->error(
            "Failed to parse JSON file {}:", filename, "JSON format error");
        m_sp_logger->error(reader_json.getFormattedErrorMessages());
        throw ExceptionParseFailure(
            m_str_description, std::string("JSON format error"));
    }
    std::string image_filename =
        pathname(filename) + m_json_tileset["image"].asString();
    std::string name = m_json_tileset["name"].asString();
    unsigned int tilewidth = m_json_tileset["tilewidth"].asInt();
    unsigned int tileheight = m_json_tileset["tilewidth"].asInt();
    unsigned int tilecount = m_json_tileset["tilecount"].asInt();
    unsigned int columns = m_json_tileset["columns"].asInt();
    unsigned int margin = m_json_tileset["margin"].asInt();
    unsigned int spacing = m_json_tileset["spacing"].asInt();
    return new LevelTileSet{name,
        image_filename,
        columns,
        tilecount,
        tilewidth,
        tileheight,
        spacing,
        margin};
}

} // namespace core
