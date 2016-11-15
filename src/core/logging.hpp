#ifndef CORE_LOGGING_HPP
#define CORE_LOGGING_HPP

#include <spdlog/spdlog.h>
#include <string>

namespace core
{
void logging_init(bool logfiles = true);

std::shared_ptr<spdlog::logger> logging_get_logger(
    const std::string loggername);
}

#endif /* CORE_LOGGING_HPP */
