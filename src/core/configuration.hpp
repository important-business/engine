#ifndef CORE_CONFIGURATION_HPP
#define CORE_CONFIGURATION_HPP

#include "common/logging.hpp"
#include <json/json.h>
#include <memory>

namespace core
{

class Configuration
{
public:
    explicit Configuration(std::string filename);
    bool get_render_acceleration() const;
    bool get_render_show_collision() const;
    bool get_render_vsync() const;
    bool get_render_collision() const;
    int get_window_height() const;
    int get_window_width() const;

private:
    const std::string m_str_filename;
    Json::Value m_json_config;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* CORE_CONFIGURATION_HPP */
