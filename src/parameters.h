#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

enum class RunMode
{
    build,
    query
};

class Parameters
{
public:
    RunMode mode;
    //std::string sequences;

    // The path to the sequence file to index. Only used in building mode.
    fs::path sequences_path;

    // The path to the database index. Only used in query mode.
    fs::path database_path;
    // The path to the query file. Only used in query mode.
    fs::path query_path;
};

#endif // _PARAMETERS_H_