#include <catch2/catch_test_macros.hpp>

#include "parameters.h"
#include "query_index.h"

#include "dummy_output_writer.h"


class DummyQueryIndexCommand: public QueryIndexCommand {
public:

    DummyQueryIndexCommand(const Parameters &params, const BaseOutputWriter &writer) :
        QueryIndexCommand(params, writer) {}

    void run() const override {}
    auto getQueryParametersPublic() const {
        return getQueryParameters();
    }
};


TEST_CASE("test base query properties", "[query]") {
 
    SECTION("thread count can be set for bruteforce") {
        // Given
        Parameters params;
        params.index_algorithm = IndexAlgorithm::bruteforce;
        params.indexThreadQty = 12;

        DummyOutputWriter writer;
        DummyQueryIndexCommand command(params, writer);

        // When
        auto queryParams = command.getQueryParametersPublic();

        // Then
        int i  = 0;
        for (; i < queryParams.ParamNames.size(); i++) {
            if (queryParams.ParamNames[i] == "threadQty") {
                break;
            }
        }
        REQUIRE(i != queryParams.ParamNames.size());
        REQUIRE(queryParams.ParamValues[i] == "12");
    }
}