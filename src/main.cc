#include <iostream>

#include "build_index.h"
#include "command_parser.h"
#include "dump_index.h"
#include "parameters.h"
#include "query_index_align.h"
#include "errors.h"

int main(int argc, char *argv[])
{
    try
    {
        Parameters params;
        params = parse_command_line_args(argc, argv);
  
        std::cerr << "Algorithm: " << IndexAlgorithmToString(params.index_algorithm) << std::endl;
   
        switch (params.mode)
        {
        case RunMode::build:
            BuildIndexCommand(params).run();
            break;
        case RunMode::query:
            AlignQueryIndexCommand(params).run();
            break;
        case RunMode::dump:
            DumpIndexCommand(params).run();
        default:
            break;
            // do nothing
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}