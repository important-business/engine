#ifndef DATA_HPP
#define DATA_HPP

#include "logging.hpp"
#include <json/json.h>
#include <string>
#include <anax/anax.hpp>

namespace core
{
class DataReader
{
public:
    DataReader(std::string filename);
    anax::Entity makeEntity(std::string entityname, anax::World& world);
    void makeEntities(anax::World& world);

private:
    const std::string m_str_filename;
    Json::Value m_json_config;
    std::shared_ptr<spdlog::logger> m_sp_logger;
    std::map<std::string, anax::Entity> m_map_entities;
};
} // namespace core
#endif /* DATA_HPP */
