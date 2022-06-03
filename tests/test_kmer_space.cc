#include <catch2/catch_test_macros.hpp>

#include "kmer_space.h"
#include "spectrum.h"

class TestingKMerSpace : public KMerSpace {
public:
    explicit TestingKMerSpace(const std::vector<Spectrum>* spectra) : KMerSpace(spectra) {}

    // Expose private function for testing
    float dist(const similarity::Object *obj1, const similarity::Object *obj2) const {
        return HiddenDistance(obj1, obj2);
    }
};


TEST_CASE("", "[kmer]" ) {
    std::vector<Spectrum> spectra = {
        Spectrum("AAAA", 2),
        Spectrum("AAAGGG", 2)
    };
    similarity::Object obj1(0, 0, 0, nullptr);
    similarity::Object obj2(1, 0, 0, nullptr);

    SECTION("use kmer space to compute cosine distance") {
        // Given
        TestingKMerSpace space(&spectra);

        // When
        float d = space.dist(&obj1, &obj2);

        // Then
        REQUIRE(d == 1.f/3);
    }

    SECTION("kmer space string representation") {
        // Given
        KMerSpace space(&spectra);

        // When/then
        REQUIRE(space.StrDesc() == "KMer cosine distance");
    }
}