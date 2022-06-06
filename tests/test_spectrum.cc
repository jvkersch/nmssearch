#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "spectrum.h"


TEST_CASE("kmer spectrum", "[spectrum]") {
    SECTION("can initalize a spectrum with a name") {
        Spectrum spec("foo", "AAAA", 2);
        REQUIRE(spec.name() == "foo");
    }

    SECTION("can build kmer spectrum from sequences") {
        const std::string sequence = "AAATTGGG";
        Spectrum spec("", sequence, 2);

        REQUIRE(spec.size() == 5);
    }

    SECTION("can convert spectrum to map") {
        const std::string sequence = "AAATTGGG";
        Spectrum spec("", sequence, 2);
        auto map = spec.to_map();

        REQUIRE(map.size() == 5);
        REQUIRE(map["AA"] == 2);
        REQUIRE(map["AT"] == 1);
        REQUIRE(map["TT"] == 1);
        REQUIRE(map["TG"] == 1);
        REQUIRE(map["GG"] == 2);
    }

    SECTION("can compute the norm (l2) of a spectrum") {
        const std::string sequence = "AAATTGGG"; // AA: 2, AT: 1, TT: 1, TG: 1, GG: 2
        Spectrum spec("", sequence, 2);

        REQUIRE(spec.norm() == Catch::Approx(3.3166247903554)); // sqrt(11)
    }

    SECTION("can compute cosine distance for simple sequences", "[spectrum]") {
        Spectrum s1("", "AAAA", 2);   // AA: 3
        Spectrum s2("", "AAAGG", 2); // AA: 2, AG: 1, GG: 1

        // Expected cosine distance: 1 - 3*2/3/sqrt(6) = 1 - sqrt(2/3) = 0.1835034191
        REQUIRE(cosine_distance(s1, s2) == Catch::Approx(0.1835034191));
    }

    SECTION("equal spectra compare equally", "[spectrum]") {
        Spectrum s1("s", "AAAA", 2);   // AA: 3
        Spectrum s2("s", "AAAA", 2);   // AA: 3
        Spectrum s3("t", "AAAAG", 2); // AA: 3, AG: 1
        Spectrum s4("u", "AAAAG", 2); // AA: 3, AG: 1

        REQUIRE(s1 == s2);
        REQUIRE(s1 != s3);
        REQUIRE(s3 != s4);
    }
}  