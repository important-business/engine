#include "exception.hpp"

namespace core
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
} // namespace core
