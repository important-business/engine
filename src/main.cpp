#include "core/application.hpp"
#include "core/logging.hpp"

#include <exception>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{
    int to_return = 0;
    core::logging_init();
    auto logger_main = core::logging_get_logger("main");
    try
    {
        core::Application app{argc, argv};
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
