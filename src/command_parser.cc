#include <iostream>
#include <sstream>
#include "CLI11.hpp"

#include "command_parser.h"
#include "errors.h"

Parameters parse_command_line_args(int argc, char **argv)
{
    CLI::App app{"App description"}; // TODO replace by something more appropriate
    app.require_subcommand(1);

    CLI::App *build_sc = app.add_subcommand("build", "build search index");

    std::string sequences;
    build_sc->add_option("sequences", sequences, "FASTA file to index"); // TODO make required

    CLI::App *query_sc = app.add_subcommand("query", "query previously-built search index");

    std::string database;
    query_sc->add_option("database", database, "Database to query"); // TODO make required
    std::string query;
    query_sc->add_option("query", query, "Query file in FASTA format"); // TODO make required

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
        p.sequences = sequences;
    }
    else if (query_sc->parsed())
    {
        p.mode = RunMode::query;
        p.database = database;
        p.query = query;
    }

    return p;
}