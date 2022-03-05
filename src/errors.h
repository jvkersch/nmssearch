#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <exception>
#include <string>

class ExitError : std::exception
{
public:
    ExitError(std::string message) : _message(message) {}
    const char *what() const throw ()
    {
        return _message.c_str();
    }

private:
    std::string _message;
};

#endif // _ERRORS_H_