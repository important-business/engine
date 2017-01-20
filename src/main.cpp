#include "core/application.hpp"
#include "core/configuration.hpp"
#include "common/logging.hpp"

#include <exception>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

const char* CONFIG_PATH{"config.json"};

int main(int argc, char* argv[])
{
    int to_return = 0;
    common::logging_init();
    auto logger_main = common::logging_get_logger("main");
    try
    {
        core::Configuration config{CONFIG_PATH};
        config.set_logging_levels();
        core::Application app{argc, argv, config};
        logger_main->info("Initializing application");
        app.init();
        to_return = app.loop();
    }
    catch (std::exception& e)
    {
        logger_main->critical("Exiting with exception:");
        logger_main->critical("\t{}", e.what());
        to_return = 1;
    }
    return to_return;
}
