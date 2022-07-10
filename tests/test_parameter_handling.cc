#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "command_parser.h"
#include "parameters.h"

#define NELEMS(arr)  (sizeof(arr) / sizeof((arr)[0]))

auto parse_cli_helper(const char *algorithm) {
    char const *argv[] = {
        "dummy",
        "--algorithm",
        algorithm,
        "dump",
        "index.bin"
    };
    int argc = NELEMS(argv);
    return parse_command_line_args(argc, argv);
}

TEST_CASE( "command-line arguments are handled", "[parameters]" ) {

    SECTION("algorithm optional parameter is set") {

        auto params = parse_cli_helper("bruteforce");
        REQUIRE(params.index_algorithm == IndexAlgorithm::bruteforce);
        params = parse_cli_helper("hnsw");
        REQUIRE(params.index_algorithm == IndexAlgorithm::hnsw);
        params = parse_cli_helper("vptree");
        REQUIRE(params.index_algorithm == IndexAlgorithm::vptree);
        params = parse_cli_helper("hnsw-kmer");
        REQUIRE(params.index_algorithm == IndexAlgorithm::hnsw_kmer);
        params = parse_cli_helper("bruteforce-kmer");
        REQUIRE(params.index_algorithm == IndexAlgorithm::bruteforce_kmer);
    }

    SECTION("algorithm optional parameter defaults to hnsw") {
        
        char const *argv[] = {
            "dummy",
            "dump",
            "index.bin"
        };
        int argc = NELEMS(argv);

        auto params = parse_command_line_args(argc, argv);
        REQUIRE(params.index_algorithm == IndexAlgorithm::hnsw);
    }

    SECTION("kmer length can be specified") {
        char const *argv[] = {
            "dummy",
            "--kmer-length", "12",
            "dump",
            "index.bin"
        };
        int argc = NELEMS(argv);

        auto params = parse_command_line_args(argc, argv);
        REQUIRE(params.kmer_length == 12);
    }
}