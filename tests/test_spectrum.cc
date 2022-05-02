#include <catch2/catch_test_macros.hpp>

#include "spectrum.h"


TEST_CASE("kmer spectrum", "[spectrum]") {
    SECTION("can build kmer spectrum from sequences") {
        const std::string sequence = "AAATTGGG";
        Spectrum spec(sequence, 2);

        REQUIRE(spec.size() == 5);
    }

    SECTION("can convert spectrum to map") {
        const std::string sequence = "AAATTGGG";
        Spectrum spec(sequence, 2);
        auto map = spec.to_map();

        REQUIRE(map.size() == 5);
        REQUIRE(map["AA"] == 2);
        REQUIRE(map["AT"] == 1);
        REQUIRE(map["TT"] == 1);
        REQUIRE(map["TG"] == 1);
        REQUIRE(map["GG"] == 2);
    }
}  