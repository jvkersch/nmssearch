#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "command_parser.h"
#include "parameters.h"

#define NELEMS(arr)  (sizeof(arr) / sizeof((arr)[0]))

TEST_CASE( "command-line arguments are handled", "[parameters]" ) {

    SECTION("algorithm optional parameter is set") {

        char const *argv[] = {
            "dummy",
            "--algorithm",
            "bruteforce",
            "dump",
            "index.bin"
        };
        int argc = NELEMS(argv);

        auto params = parse_command_line_args(argc, argv);
        REQUIRE(params.index_algorithm == IndexAlgorithm::bruteforce);
    }

    SECTION("algorithm optional parameter defaults to vptree") {
        
        char const *argv[] = {
            "dummy",
            "dump",
            "index.bin"
        };
        int argc = NELEMS(argv);

        auto params = parse_command_line_args(argc, argv);
        REQUIRE(params.index_algorithm == IndexAlgorithm::hnsw);
    }
}