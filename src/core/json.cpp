#include "json.hpp"
#include "exception.hpp"

#include <iostream>
#include <fstream>
#include <map>

namespace core
{

void JsonReader::check_required_component_property(
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

JsonReader::JsonReader(std::string filename) : m_str_filename(filename)
{
    m_sp_logger = logging_get_logger("json");

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


void JsonReader::scan_references(Json::Value data)
{
    if (data.isObject()){
        Json::Value::Members member_names = data.getMemberNames();
        for (auto it = member_names.begin(); it != member_names.end(); ++it)
        {
            m_sp_logger->debug("Scanning references at {}", *it);
            if (it->at(0) == '$'){
                m_sp_logger->info("{} is a reference!", *it);
                m_map_references.insert(std::make_pair(*it, data[*it]));
            }
            scan_references(data[*it]);
        }
    }
    m_sp_logger->debug("Not object, stopping recursion");
}

Json::Value JsonReader::merge_values(Json::Value data, Json::Value overlay){
    Json::Value result = data;
    m_sp_logger->debug("Performing merge");
    m_sp_logger->debug("Data:");
    m_sp_logger->debug(data.toStyledString());
    m_sp_logger->debug("Overlay:");
    m_sp_logger->debug(overlay.toStyledString());
    if (data.isObject() and overlay.isObject()){
        m_sp_logger->debug("Object detected");
        Json::Value::Members member_names = overlay.getMemberNames();
        for (auto it = member_names.begin(); it != member_names.end(); ++it)
        {
            if(data.isMember(*it)){
                m_sp_logger->debug("Found common member {}", *it);
                result[*it] = merge_values(result[*it], overlay[*it]);
            }else{
                m_sp_logger->debug("Found unique member {}", *it);
                result[*it] = (overlay[*it]);
            }
        }
    }else if (data.isArray() and overlay.isArray()){
        m_sp_logger->debug("Array detected");
        for (auto it = overlay.begin(); it != overlay.end(); ++it)
        {
            result.append(*it);
        }
    }else{
        m_sp_logger->debug("Other object type detected - overwriting");
        result = overlay;
    }
    m_sp_logger->debug("Output:");
    m_sp_logger->debug(result.toStyledString());
    return result;
}

} // namespace core
