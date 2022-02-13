#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

enum class RunMode
{
    build,
    query,
    dump
};

enum class IndexAlgorithm
{
    vptree,
    hnsw
};

class Parameters
{
public:
    RunMode mode;

    // The algorithm to be used for indexing and querying.
    IndexAlgorithm index_algorithm;
    // Whether to run with instrumentation, i.e. logging all calls to distance function.
    bool instrumentation;

    // The path to the sequence file to index. Only used in building mode.
    fs::path sequences_path;
    // The path to the database index. Used in building and query mode.
    fs::path database_path;
    // The path to the query file. Only used in query mode.
    fs::path query_path;
    // The number of query results to return. Only used in query mode.
    int k = 4;
};

constexpr const char *IndexAlgorithmToString(IndexAlgorithm algorithm) throw()
{
    switch (algorithm)
    {
    case IndexAlgorithm::vptree:
        return "vantage point tree";
    case IndexAlgorithm::hnsw:
        return "HNSW";
    default:
        throw std::invalid_argument("Invalid algorithm");
    }
}

#endif // _PARAMETERS_H_