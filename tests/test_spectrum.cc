#include <catch2/catch_test_macros.hpp>

#include "spectrum.h"


TEST_CASE("kmer spectrum", "[spectrum]") {
    SECTION("can build kmer spectrum from sequences") {
        const std::string sequence = "AAATTGGG";
        Spectrum spec(sequence, 2);

        REQUIRE(spec.size() == 5);
    }
}  