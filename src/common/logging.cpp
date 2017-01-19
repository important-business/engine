#include "common/logging.hpp"

#include <iostream>

namespace core
{

std::shared_ptr<spdlog::logger> log_logger;
std::vector<spdlog::sink_ptr> sinks;

void logging_init(bool logfiles)
{
    try
    {
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
        if (logfiles)
        {
            sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>(
                "logfile", "log", 23, 59));
        }
        log_logger = std::make_shared<spdlog::logger>(
            "logging", begin(sinks), end(sinks));
        spdlog::register_logger(log_logger);
        log_logger->info("Logging initialized");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log failed: " << ex.what() << std::endl;
    }
}

std::shared_ptr<spdlog::logger> logging_get_logger(
    const std::string& loggername)
{
    /* return std::make_shared<spdlog::logger>(nullptr); */
    log_logger->info("Making logger {}", loggername);
    auto logger =
        std::make_shared<spdlog::logger>(loggername, begin(sinks), end(sinks));
    return logger;
}
} // namespace core
