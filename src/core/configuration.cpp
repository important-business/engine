#include "configuration.hpp"
#include <fstream>

namespace core
{

const int WINDOW_WIDTH_DEFAULT{640};
const int WINDOW_HEIGHT_DEFAULT{480};
const bool RENDER_ACCEL_DEFAULT{false};
const bool RENDER_SHOW_COLLISION_DEFAULT{false};
const bool RENDER_VSYNC_DEFAULT{false};

Configuration::Configuration(std::string filename)
    : m_str_filename(filename),
      m_sp_logger(common::logging_get_logger("config"))
{
    Json::Reader reader_json;
    m_sp_logger->info("Loading configuration from {}", filename);
    std::ifstream config_file(filename, std::ifstream::binary);
    reader_json.parse(config_file, m_json_config);
}

bool Configuration::get_render_acceleration() const
{
    return m_json_config["render"]
        .get("acceleration", RENDER_ACCEL_DEFAULT)
        .asBool();
}

bool Configuration::get_render_show_collision() const
{
    return m_json_config["render"]
        .get("show_collision", RENDER_SHOW_COLLISION_DEFAULT)
        .asBool();
}

bool Configuration::get_render_vsync() const
{
    return m_json_config["render"].get("vsync", RENDER_VSYNC_DEFAULT).asBool();
}

int Configuration::get_window_height() const
{
    return m_json_config["window"].get("size_y", WINDOW_HEIGHT_DEFAULT).asInt();
}

int Configuration::get_window_width() const
{
    return m_json_config["window"].get("size_x", WINDOW_WIDTH_DEFAULT).asInt();
}

} // namespace core
