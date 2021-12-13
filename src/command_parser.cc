#include <iostream>
#include <filesystem>
#include <sstream>
#include "CLI11.hpp"

#include "command_parser.h"
#include "errors.h"

namespace fs = std::filesystem;

IndexAlgorithm stringToIndexAlgorithm(const std::string &algorithm)
{
    if (algorithm == "vptree")
    {
        return IndexAlgorithm::vptree;
    }
    else if (algorithm == "hnsw")
    {
        return IndexAlgorithm::hnsw;
    }
    else
    {
        throw ExitError("invalid algorithm", -1); // TODO make nice
    }
}

Parameters parse_command_line_args(int argc, char **argv)
{
    CLI::App app{"Sequence indexing and querying using NMSLIB"};
    app.require_subcommand(1);

    std::string algorithm;
    app.add_option("--algorithm", algorithm, "algorithm to use for searching/indexing.");

    CLI::App *build_sc = app.add_subcommand("build", "build search index");

    fs::path sequences_path;
    build_sc->add_option("sequences", sequences_path, "FASTA file to index")->required();

    CLI::App *query_sc = app.add_subcommand("query", "query previously-built search index");

    fs::path database_path;
    query_sc->add_option("database", database_path, "Database to query")->required();
    fs::path query_path;
    query_sc->add_option("query", query_path, "Query file in FASTA format")->required();

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
        std::stringstream ss;
        int exitcode = app.exit(e, ss, ss);

        throw ExitError(ss.str(), exitcode);
    }
    catch (...)
    {
        throw ExitError("An unknown error occurred while parsing command-line options.", -1);
    }

    Parameters p;

    if (build_sc->parsed())
    {
        p.mode = RunMode::build;
        p.index_algorithm = stringToIndexAlgorithm(algorithm);
        p.sequences_path = sequences_path;
    }
    else if (query_sc->parsed())
    {
        p.mode = RunMode::query;
        p.index_algorithm = stringToIndexAlgorithm(algorithm);
        p.database_path = database_path;
        p.query_path = query_path;
    }

    return p;
}