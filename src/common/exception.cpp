/**
 * @file
 * @brief Common Exception source
 *
 * Implements a common base exception and some other common exceptions
 */

#include "common/exception.hpp"

namespace common
{

const char* Exception::what() const throw()
{
    return m_message.c_str();
}

ExceptionParseFailure::ExceptionParseFailure(
    std::string filename, std::string msg = std::string(""))
    : Exception("Failed parsing file '")
{
    m_message.append(filename);
    m_message.append("\' - ");
    m_message.append(msg);
}

} // namespace common
