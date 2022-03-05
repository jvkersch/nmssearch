#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <exception>
#include <string>

class Error : std::exception
{
public:
    Error(std::string message) : m_message(message) {}
    const char *what() const throw ()
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

#endif // _ERRORS_H_