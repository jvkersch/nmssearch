#include <catch2/catch_test_macros.hpp>

#include "nw_align.h"

TEST_CASE("needleman-wunsch aligner", "[nw-align]") {

    SECTION("default parameters") {
        // Given
        NeedlemanWunschAligner align1;

        // When
        int score = align1.align("AAAGG", 5, "AAGG", 4);

        // Then
        REQUIRE(score == 29);
    }

    SECTION("custom parameters") {
        // Given
        NeedlemanWunschAligner align1(1, 1);

        // When
        int score = align1.align("AAAGG", 5, "AAGG", 4);

        // Then
        REQUIRE(score == 7);
    }

}