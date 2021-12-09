#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <exception>
#include <string>

class ExitError : std::exception
{
public:
    ExitError(std::string message, int exitcode) : _message(message), _exitcode(exitcode) {}
    const char *what() const throw ()
    {
        return _message.c_str();
    }
    int exitcode() const
    {
        return _exitcode;
    }

private:
    std::string _message;
    int _exitcode;
};

#endif // _ERRORS_H_