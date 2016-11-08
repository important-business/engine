#ifndef CORE_CONFIGURATION_HPP
#define CORE_CONFIGURATION_HPP

#include "logging.hpp"
#include <json/json.h>
#include <memory>

namespace core
{

class Configuration
{
public:
    Configuration(std::string filename);
    int get_window_height();
    int get_window_width();

private:
    const std::string m_str_filename;
    Json::Value m_json_config;
    std::shared_ptr<spdlog::logger> m_sp_logger;
};
}

#endif /* CORE_CONFIGURATION_HPP */
