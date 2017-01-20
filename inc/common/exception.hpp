#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

namespace common
{

class Exception : public std::exception
{
public:
    explicit Exception(std::string message) : m_message(message)
    {
    }

    virtual const char* what() const throw();

    virtual ~Exception(){}

protected:
    std::string m_message;
};

class ExceptionParseFailure : public Exception
{
public:
    ExceptionParseFailure(std::string filename, std::string msg);
};


} // namespace common
#endif /* EXCEPTION_HPP */
