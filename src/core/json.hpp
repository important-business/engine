#ifndef JSON_HPP
#define JSON_HPP

#include "logging.hpp"
#include <json/json.h>
#include <string>

namespace core
{

class JsonReader
{
public:
    JsonReader(std::string filename);

    void write_file(std::string filename);

protected:
    void check_required_component_property(
        const Json::Value data, std::string component, std::string property);

    void scan_references(Json::Value data);

    Json::Value merge_values(Json::Value data, Json::Value overlay);

    const std::string m_str_filename;
    Json::Value m_json_config;
    std::map<std::string, Json::Value> m_map_references;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};

} // namespace core
#endif /* JSON_HPP */
