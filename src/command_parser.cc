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
        std::stringstream ss;
        ss << "Invalid algorithm: " << algorithm;
        throw ExitError(ss.str(), -1);
    }
}

void AddToplevelArguments(CLI::App &app, Parameters &params)
{
    app.add_option_function<std::string>(
        "--algorithm",
        [&params](const std::string &algorithm)
        {
            params.index_algorithm = stringToIndexAlgorithm(algorithm);
        },
        "algorithm to use for searching/indexing.");
}

CLI::App *AddBuildSubCommand(CLI::App &app, Parameters &params)
{
    CLI::App *build_sc = app.add_subcommand("build", "build search index");

    build_sc
        ->add_option("sequences", params.sequences_path, "FASTA file to index")
        ->required();
    build_sc
        ->add_option("database", params.database_path, "Output database")
        ->required();

    return build_sc;
}

CLI::App *AddQuerySubCommand(CLI::App &app, Parameters &params)
{
    CLI::App *query_sc = app.add_subcommand("query", "query previously-built search index");

    query_sc
        ->add_option("database", params.database_path, "Database to query")
        ->required();
    query_sc
        ->add_option("query", params.query_path, "Query file in FASTA format")
        ->required();
    query_sc
        ->add_option("-k", params.k, "Number of results to return per query.");

    return query_sc;
}

CLI::App *AddDumpSubCommand(CLI::App &app, Parameters &params)
{
    CLI::App *dump_sc = app.add_subcommand("dump", "dump previously-built search index to JSON");

    dump_sc
        ->add_option("database", params.database_path, "Database to query")
        ->required();

    return dump_sc;
}

Parameters parse_command_line_args(int argc, char **argv)
{
    CLI::App app{"Sequence indexing and querying using NMSLIB"};
    app.require_subcommand(1);

    Parameters p;

    AddToplevelArguments(app, p);
    auto build_sc = AddBuildSubCommand(app, p);
    auto query_sc = AddQuerySubCommand(app, p);
    auto dump_sc = AddDumpSubCommand(app, p);

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

    if (build_sc->parsed())
    {
        p.mode = RunMode::build;
    }
    else if (query_sc->parsed())
    {
        p.mode = RunMode::query;
    }
    else if (dump_sc->parsed())
    {
        p.mode = RunMode::dump;
    }

    return p;
}