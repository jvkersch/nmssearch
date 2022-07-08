#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <algorithm>
#include <sstream>
#include <vector>

#include "output_writer.h"
#include "parameters.h"
#include "query_index_align.h"

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


TEST_CASE("Query the index in align mode", "[query-align]") {

    // Write sequence database
    std::stringstream s;
    s << ">A\n"
       << "AAATTGG\n"
       << ">B\n"
       << "AAAGGT\n";
    auto database = CreateTempFile(s.str());

    // Write query file
    std::stringstream t;
    t << ">q\n"
      << "AAAACGT\n";
    auto query = CreateTempFile(t.str());

    SECTION("bruteforce") {
        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::bruteforce;
        params.database_path = database;
        params.query_path = query;
        DummyOutputWriter writer;

        AlignQueryIndexCommand cmd(params, writer);

        // When
        cmd.run();

        // Then
        REQUIRE(writer.ncalls() == 2);
        REQUIRE(writer[0].subjectId == "A");
        REQUIRE(writer[0].pctIdentity == Catch::Approx(57.14).margin(0.01));
        REQUIRE(writer[1].subjectId == "B");
        REQUIRE(writer[1].pctIdentity == Catch::Approx(83.33).margin(0.01));
    }

    RemoveTempFile(query);
    RemoveTempFile(database);
}