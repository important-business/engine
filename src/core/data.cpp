#include "components/camera.hpp"
#include "components/collision.hpp"
#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "data.hpp"
#include "exception.hpp"

#include <cassert>
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

std::string basename(const std::string& pathname)
{
    // TODO(Keegan, Check if works on Windows)
    // TODO(Keegan, Test properly)
    return {std::find_if(pathname.rbegin(),
                pathname.rend(),
                [](char c) { return (c == '/' || c == '\\'); })
                .base(),
        pathname.end()};
}

std::string pathname(const std::string& pathname)
{
    // TODO(Keegan, Check if works on Windows)
    // TODO(Keegan, Test properly)
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
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
                [i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c : in)
    {
        if (T[c] == -1)
            break;
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
    if (in.size() % 4)
    {
        // TODO(Keegan, Handle error better)
        assert(false);
    }
    for (auto it = in.begin(); it < in.end(); it += 4)
    {
        auto b1 = *(it);
        auto b2 = *(it + 1);
        auto b3 = *(it + 2);
        auto b4 = *(it + 3);
        int32_t val = b1;
        // TODO(Keegan, This is awfully endian dependent...)
        val |= b2 << 8;
        val |= b3 << 16;
        val |= b4 << 24;
        result.push_back(val);
    }
    return result;
}

void DataReader::factory_component_player(
    const Json::Value data, anax::Entity entity)
{
    const std::string prop_move_accel{"move_accel"};
    const std::string prop_top_speed{"top_speed"};

    check_required_component_property(
        data, component_name_player, prop_move_accel);
    float move_accel = data[prop_move_accel].asFloat();
    check_required_component_property(
        data, component_name_player, prop_top_speed);
    float top_speed = data[prop_top_speed].asFloat();
    entity.addComponent<components::PlayerComponent>(move_accel, top_speed);
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

    float velocity_x = data.get(prop_vel_x, prop_vel_x_default).asFloat();
    float velocity_y = data.get(prop_vel_y, prop_vel_y_default).asFloat();
    float force_x = data.get(prop_force_x, prop_force_x_default).asFloat();
    float force_y = data.get(prop_force_y, prop_force_y_default).asFloat();
    check_required_component_property(data, component_name_velocity, prop_mass);
    float mass = data[prop_mass].asFloat();

    check_required_component_property(
        data, component_name_velocity, prop_friction);
    float friction = data[prop_friction].asFloat();

    entity.addComponent<components::VelocityComponent>(
        mass, friction, velocity_x, velocity_y, force_x, force_y);
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

LevelReader::LevelReader(std::string filename) : JsonFileReader(filename)
{
    m_sp_logger = logging_get_logger("data");
}

void LevelReader::build_level(std::unique_ptr<Level>& up_level)
{
    uint16_t size_x = m_json_data["width"].asInt();
    uint16_t size_y = m_json_data["height"].asInt();
    uint16_t layer_count = m_json_data["layers"].size();
    float tileheight = m_json_data["tileheight"].asFloat();
    float scale = m_json_data["properties"].get("scale", 1.0).asFloat();

    up_level = std::make_unique<Level>(
        size_x, size_y, layer_count, tileheight * scale);
    auto p_level = up_level.get();

    // TODO(Keegan): Use tilewidth as well
    auto tilesets = m_json_data["tilesets"];
    std::string cur_path = pathname(m_str_description);
    std::string tileset_source;
    for (auto it = tilesets.begin(); it != tilesets.end(); ++it)
    {
        int firstgid = (*it)["firstgid"].asInt();
        tileset_source = cur_path + (*it)["source"].asString();
        m_sp_logger->info(
            "Found a tileset gid {} source {}", firstgid, tileset_source);
    }
    // Only load last tileset for now
    auto p_tileset = load_tileset(tileset_source);
    p_level->set_tileset(p_tileset);

    auto layers = m_json_data["layers"];
    for (int16_t layerindex = 0; layerindex < layers.size(); ++layerindex)
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
        // TODO(Keegan, check encoding, don't just assume)
        auto decoded = string_decode(base64_decode(data.asString()));
        for (uint16_t index = 0; index < decoded.size(); ++index)
        {
            uint16_t tile_x = index % width;
            uint16_t tile_y = index / width;
            int16_t tilegid = decoded[index];
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
    uint16_t tilewidth = m_json_tileset["tilewidth"].asInt();
    uint16_t tileheight = m_json_tileset["tilewidth"].asInt();
    uint16_t tilecount = m_json_tileset["tilecount"].asInt();
    uint16_t columns = m_json_tileset["columns"].asInt();
    uint16_t margin = m_json_tileset["margin"].asInt();
    uint16_t spacing = m_json_tileset["spacing"].asInt();
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
