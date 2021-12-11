#include <iostream>

#include "build_index.h"
#include "command_parser.h"
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

    switch (params.mode)
    {
    case RunMode::build:
        BuildIndexCommand(params).run();
        break;
    case RunMode::query:
        QueryIndexCommand(params).run();
        break;
    default:
        break;
        // do nothing
    }

    return 0;
}