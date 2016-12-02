#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

namespace core
{

class Exception : public std::exception
{
public:
    Exception(std::string message) : m_message(message)
    {
    }

    virtual const char* what() const throw();

protected:
    std::string m_message;
};

class ExceptionParseFailure : public Exception
{
public:
    ExceptionParseFailure(std::string filename, std::string msg);
};


class ExceptionMissingComponent : public Exception
{
public:
    ExceptionMissingComponent(std::string component, std::string entityname = "");
};
} // namespace core
#endif /* EXCEPTION_HPP */
