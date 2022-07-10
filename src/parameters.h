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
    hnsw,
    bruteforce,
    hnsw_kmer,
    bruteforce_kmer
};

class Parameters
{
public:
    RunMode mode;

    // The algorithm to be used for indexing and querying.
    IndexAlgorithm index_algorithm;
    // Whether to run with instrumentation, i.e. logging all calls to distance function.
    bool instrumentation;
    // Number of threads to use
    int indexThreadQty = 4;

    // The path to the sequence file to index. Only used in building mode.
    fs::path sequences_path;
    // The path to the database index. Used in building and query mode.
    fs::path database_path;
    // The path to the query file. Only used in query mode.
    fs::path query_path;

    // Number of query results to return. Only used in query mode.
    int k = 4;
    // Number of bidirectional links (see HNSW documentation).
    int M = 16;  
    // Size of neighbor lists during construction (see HNSW documentation).
    int efConstruction = 200;
    // KMer length for spectral-based methods;
    int kmer_length = 7;
};

constexpr const char *IndexAlgorithmToString(IndexAlgorithm algorithm)
{
    switch (algorithm)
    {
    case IndexAlgorithm::vptree:
        return "vantage point tree";
    case IndexAlgorithm::hnsw:
        return "HNSW";
    case IndexAlgorithm::bruteforce:
        return "Bruteforce";
    default:
        throw std::invalid_argument("Invalid algorithm");
    }
}

#endif // _PARAMETERS_H_