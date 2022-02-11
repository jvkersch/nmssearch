#include <iostream>

#include "build_index.h"
#include "command_parser.h"
#include "dump_index.h"
#include "parameters.h"
#include "query_index.h"
#include "errors.h"

int main(int argc, char *argv[])
{
    Parameters params;

    try
    {
        params = parse_command_line_args(argc, argv);
    }
    catch (ExitError &e)
    {
        std::cerr << e.what();
        return e.exitcode();
    }

    std::cerr << "Algorithm: " << IndexAlgorithmToString(params.index_algorithm) << std::endl;

    switch (params.mode)
    {
    case RunMode::build:
        BuildIndexCommand(params).run();
        break;
    case RunMode::query:
        QueryIndexCommand(params).run();
        break;
    case RunMode::dump:
        DumpIndexCommand(params).run();
    default:
        break;
        // do nothing
    }

    return 0;
}