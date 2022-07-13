#include <iostream>

#include "build_index_align.h"
#include "build_index_spectrum.h"
#include "command_parser.h"
#include "dump_index.h"
#include "output_writer.h"
#include "parameters.h"
#include "query_index_align.h"
#include "query_index_spectrum.h"
#include "errors.h"

int main(int argc, char *argv[])
{
    try
    {
        Parameters params;
        params = parse_command_line_args(argc, argv);
  
        std::cerr << "Algorithm: " << IndexAlgorithmToString(params.index_algorithm) << std::endl;
   
        OutputWriter writer;
        switch (params.mode)
        {
        case RunMode::build:
            // FIXME: Replace by factory (here and below)
            if ((params.index_algorithm == IndexAlgorithm::bruteforce_kmer) || (params.index_algorithm == IndexAlgorithm::hnsw_kmer)) {
                SpectrumBuildIndexCommand(params).run();
            } else {
                AlignBuildIndexCommand(params).run();
            }
            break;
        case RunMode::query:
            if ((params.index_algorithm == IndexAlgorithm::bruteforce_kmer) || (params.index_algorithm == IndexAlgorithm::hnsw_kmer)) {
                SpectrumQueryIndexCommand(params, writer).run();
            } else {
                AlignQueryIndexCommand(params, writer).run();
            }
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