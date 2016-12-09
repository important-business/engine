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
    explicit JsonReader(
        std::string json_string, std::string description = std::string(""));

    void write_file(std::string filename);

protected:
    void check_required_component_property(
        const Json::Value data, std::string component, std::string property);

    void scan_references(Json::Value data);

    Json::Value merge_values(Json::Value data, Json::Value overlay);

    Json::Value m_json_data;
    std::map<std::string, Json::Value> m_map_references;
    const std::string m_str_description;

    std::shared_ptr<spdlog::logger> m_sp_logger;
};

class JsonFileReader : public JsonReader
{
public:
    explicit JsonFileReader(std::string filename);

protected:
private:
    std::string load_file(std::string filename);
};

} // namespace core
#endif /* JSON_HPP */
