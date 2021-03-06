#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "parameters.h"
#include "build_index_spectrum.h"

#include "testing_helpers.h"

namespace fs = std::filesystem;


TEST_CASE("can build an index (spectrum mode)", "[build-index]") {
    
    SECTION("hnsw-spectrum") {

        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::hnsw_kmer;
        params.sequences_path = testing_artifact("seqs-small.fa");
        params.database_path = "seqs-small-kmer-tmp.bin";
        SpectrumBuildIndexCommand command(params);

        // When
        command.run();

        // Then
        require_exists(params.database_path / "spectra.bin");
        require_exists(params.database_path / "index.bin");
        require_exists(params.database_path / "sequences.fa");
        fs::remove_all(params.database_path);
    }

    SECTION("bruteforce (spectrum)") {

        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::bruteforce_kmer;
        params.sequences_path = testing_artifact("seqs-small.fa");
        params.database_path = "seqs-small-kmer-tmp.bin";
        SpectrumBuildIndexCommand command(params);

        // When
        command.run();

        // Then
        require_exists(params.database_path / "spectra.bin");
        require_exists(params.database_path / "sequences.fa");
        fs::remove_all(params.database_path);
    }
}