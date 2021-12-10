#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <string>

enum class RunMode
{
    build,
    query
};

class Parameters
{
public:
    RunMode mode;
    std::string sequences;

    std::string database;
    std::string query;
};

#endif // _PARAMETERS_H_