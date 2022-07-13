#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "parameters.h"
#include "build_index.h"

#include "testing_helpers.h"

namespace fs = std::filesystem;

void require_exists(fs::path p) {
    REQUIRE(fs::exists(p));
}

TEST_CASE("can build an index", "[build-index]") {
    
    SECTION("hnsw") {

        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::hnsw;
        params.sequences_path = testing_artifact("seqs-small.fa");
        params.database_path = "seqs-small-tmp.bin";
        BuildIndexCommand command(params);

        // When
        command.run();

        // Then
        require_exists(params.database_path / "index.bin");
        require_exists(params.database_path / "sequences.fa");
        fs::remove_all(params.database_path);
    }

    SECTION("vptree") {

        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::vptree;
        params.sequences_path = testing_artifact("seqs-small.fa");
        params.database_path = "seqs-small-tmp.bin";
        BuildIndexCommand command(params);

        // When
        command.run();

        // Then
        require_exists(params.database_path / "index.bin");
        require_exists(params.database_path / "sequences.fa");
        fs::remove_all(params.database_path);
    }
}