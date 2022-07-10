#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <algorithm>
#include <sstream>
#include <vector>

#include "output_writer.h"
#include "parameters.h"
#include "query_index_spectrum.h"

#include "testing_helpers.h"

class DummyOutputWriter: public BaseOutputWriter {
public:
    void display(const AlignStats &r, std::ostream& output) const override {
        m_stats.push_back(r);
    }
    void display(const std::vector<AlignStats>& results, std::ostream& output) const override {
        m_stats.resize(m_stats.size() + results.size());
        std::copy(results.begin(), results.end(), m_stats.rbegin());
    }

    size_t ncalls() const {
        return m_stats.size();
    }

    AlignStats operator[](size_t i) const {
        return m_stats[i];
    }

private:
    mutable std::vector<AlignStats> m_stats;
};


TEST_CASE("Query the index in spectrum mode", "[query-spectrum]") {

    SECTION("hnsw") {
        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::hnsw;
        params.database_path = testing_artifact("hnsw-spectrum-small.bin");
        params.query_path = testing_artifact("query-small.fa");
        params.kmer_length = 2;
        DummyOutputWriter writer;

        SpectrumQueryIndexCommand cmd(params, writer);

        // When
        cmd.run();

        // Then
        REQUIRE(writer.ncalls() == 2);
        REQUIRE(writer[0].subjectId == "A");
        REQUIRE(writer[0].pctIdentity == Catch::Approx(0.612).margin(0.01));
        REQUIRE(writer[1].subjectId == "B");
        REQUIRE(writer[1].pctIdentity == Catch::Approx(0.763).margin(0.01));
    }
}
