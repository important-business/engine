#include "json.hpp"
#include "exception.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
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
            m_str_description, "JSON Data component missing property");
    }
}

JsonReader::JsonReader(std::string json_string, std::string description):m_str_description(description)
{
    m_sp_logger = logging_get_logger("json");

    Json::Reader reader_json;
    if (!reader_json.parse(json_string, m_json_data))
    {
        m_sp_logger->error(
            "Failed to parse JSON {}:", m_str_description, "JSON format error");
        m_sp_logger->error(reader_json.getFormattedErrorMessages());
        throw ExceptionParseFailure(
            m_str_description, std::string("JSON format error"));
    }
}

void JsonReader::scan_references(Json::Value data)
{
    if (data.isObject())
    {
        Json::Value::Members member_names = data.getMemberNames();
        for (auto name : member_names)
        {
            m_sp_logger->debug("Scanning references at {}", name);
            if (name.at(0) == '$')
            {
                m_sp_logger->info("{} is a reference!", name);
                m_map_references.insert(std::make_pair(name, data[name]));
            }
            scan_references(data[name]);
        }
    }
    m_sp_logger->debug("Not object, stopping recursion");
}

Json::Value JsonReader::merge_values(Json::Value data, Json::Value overlay)
{
    Json::Value result = data;
    m_sp_logger->debug("Performing merge");
    m_sp_logger->debug("Data:");
    m_sp_logger->debug(data.toStyledString());
    m_sp_logger->debug("Overlay:");
    m_sp_logger->debug(overlay.toStyledString());
    if (data.isObject() and overlay.isObject())
    {
        m_sp_logger->debug("Object detected");
        Json::Value::Members member_names = overlay.getMemberNames();
        for (auto name : member_names)
        {
            if (data.isMember(name))
            {
                m_sp_logger->debug("Found common member {}", name);
                result[name] = merge_values(result[name], overlay[name]);
            }
            else
            {
                m_sp_logger->debug("Found unique member {}", name);
                result[name] = (overlay[name]);
            }
        }
    }
    else if (data.isArray() and overlay.isArray())
    {
        m_sp_logger->debug("Array detected");
        for (auto val : overlay)
        {
            result.append(val);
        }
    }
    else
    {
        m_sp_logger->debug("Other object type detected - overwriting");
        result = overlay;
    }
    m_sp_logger->debug("Output:");
    m_sp_logger->debug(result.toStyledString());
    return result;
}

void JsonReader::write_file(std::string filename)
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "    ";
    std::unique_ptr<Json::StreamWriter> writer{builder.newStreamWriter()};
    std::ofstream of{filename};
    writer->write(m_json_data, &of);
}

JsonFileReader::JsonFileReader(std::string filename) : JsonReader(load_file(filename), filename)
{}

std::string JsonFileReader::load_file(std::string filename){
    std::ifstream json_file(filename, std::ifstream::binary);
    std::stringstream sstream;
    sstream << json_file.rdbuf();
    return sstream.str();
}

} // namespace core
