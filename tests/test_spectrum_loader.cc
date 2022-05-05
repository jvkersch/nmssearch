#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <sstream>

#include "spectrum_loader.h"
#include "testing_helpers.h"


TEST_CASE("load spectra", "[spectrum_container]")
{
    std::stringstream ss;
    ss << ">A\n"
       << "AAATTGG\n"
       << ">B\n"
       << "AAAGGT\n";
    auto tempfile = CreateTempFile(ss.str());

    SECTION("Can load spectra from file")
    {
        auto spectra = LoadSpectraFromFile(tempfile, 2);
        REQUIRE(spectra.size() == 2);
        REQUIRE(spectra[0].size() == 5);
        REQUIRE(spectra[1].size() == 4);
    }

    RemoveTempFile(tempfile);
}