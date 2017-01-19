#include "configuration.hpp"
#include <fstream>

namespace core
{

const int WINDOW_WIDTH_DEFAULT{640};
const int WINDOW_HEIGHT_DEFAULT{480};

Configuration::Configuration(std::string filename) : m_str_filename(filename)
{
    m_sp_logger = logging_get_logger("config");
    Json::Reader reader_json;
    m_sp_logger->info("Loading configuration from {}", filename);
    std::ifstream config_file(filename, std::ifstream::binary);
    reader_json.parse(config_file, m_json_config);
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
