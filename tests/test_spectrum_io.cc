#include <catch2/catch_test_macros.hpp>

#include "spectrum.h"
#include "spectrum_io.h"
#include "testing_helpers.h"


TEST_CASE( "Serialize/deserialize spectrum", "[io]" ) {
    // Given
    const std::string sequence = "AAATTGGG";
    Spectrum spec(sequence, 2);

    // When
    serialize(spec, "spectrum.bin");
    auto loaded = deserialize("spectrum.bin");
    RemoveTempFile("spectrum.bin");    

    // Then
    REQUIRE(loaded.size() == 1);
    REQUIRE(spec == loaded[0]);
}

TEST_CASE( "Serialize/deserialize multiple spectrums", "[io]" ) {
    // Given
    const std::string sequence = "AAATTGGG";
    Spectrum spec1(sequence, 2);
    Spectrum spec2(sequence, 3);
    std::vector<Spectrum> spectra = {spec1, spec2};

    // When
    serialize(spectra, "spectrum.bin");
    auto loaded = deserialize("spectrum.bin");
    RemoveTempFile("spectrum.bin");    

    // Then
    REQUIRE(loaded.size() == 2);
    REQUIRE(spec1 == loaded[0]);
    REQUIRE(spec2 == loaded[1]);
}