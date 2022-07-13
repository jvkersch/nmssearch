#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <sstream>
#include <vector>

#include "parameters.h"
#include "query_index_spectrum.h"

#include "dummy_output_writer.h"
#include "testing_helpers.h"


TEST_CASE("Query the index in spectrum mode", "[query-spectrum]") {

    SECTION("hnsw") {
        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::hnsw_kmer;
        params.database_path = testing_artifact("hnsw-spectrum-small.bin");
        params.query_path = testing_artifact("query-small.fa");
        params.kmer_length = 2;
        DummyOutputWriter writer;

        SpectrumQueryIndexCommand cmd(params, writer);

        // When
        cmd.run();

        // Then
        REQUIRE(writer.ncalls() == 2);
        REQUIRE(writer[0].subjectId == "B");
        REQUIRE(writer[0].pctIdentity == Catch::Approx(0.763).margin(0.01));
        REQUIRE(writer[1].subjectId == "A");
        REQUIRE(writer[1].pctIdentity == Catch::Approx(0.612).margin(0.01));
    }
}
