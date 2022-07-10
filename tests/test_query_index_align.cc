#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <sstream>
#include <vector>

#include "parameters.h"
#include "query_index_align.h"

#include "dummy_output_writer.h"
#include "testing_helpers.h"


TEST_CASE("Query the index in align mode", "[query-align]") {

    SECTION("bruteforce") {
        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::bruteforce;
        params.database_path = testing_artifact("seqs-small.fa");
        params.query_path = testing_artifact("query-small.fa");
        DummyOutputWriter writer;

        AlignQueryIndexCommand cmd(params, writer);

        // When
        cmd.run();

        // Then
        REQUIRE(writer.ncalls() == 2);
        REQUIRE(writer[0].subjectId == "B");
        REQUIRE(writer[0].pctIdentity == Catch::Approx(83.33).margin(0.01));
        REQUIRE(writer[1].subjectId == "A");
        REQUIRE(writer[1].pctIdentity == Catch::Approx(57.14).margin(0.01));
    }

    SECTION("vptree") {
        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::vptree;
        params.database_path = testing_artifact("vptree-small.bin");
        params.query_path = testing_artifact("query-small.fa");
        DummyOutputWriter writer;

        AlignQueryIndexCommand cmd(params, writer);

        // When
        cmd.run();

        // Then
        REQUIRE(writer.ncalls() == 2);
        REQUIRE(writer[0].subjectId == "B");
        REQUIRE(writer[0].pctIdentity == Catch::Approx(83.33).margin(0.01));
        REQUIRE(writer[1].subjectId == "A");
        REQUIRE(writer[1].pctIdentity == Catch::Approx(57.14).margin(0.01));
    }

    SECTION("hnsw") {
        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::hnsw;
        params.database_path = testing_artifact("hnsw-small.bin");
        params.query_path = testing_artifact("query-small.fa");
        DummyOutputWriter writer;

        AlignQueryIndexCommand cmd(params, writer);

        // When
        cmd.run();

        // Then
        REQUIRE(writer.ncalls() == 2);
        REQUIRE(writer[0].subjectId == "B");
        REQUIRE(writer[0].pctIdentity == Catch::Approx(83.33).margin(0.01));
        REQUIRE(writer[1].subjectId == "A");
        REQUIRE(writer[1].pctIdentity == Catch::Approx(57.14).margin(0.01));
    }
}