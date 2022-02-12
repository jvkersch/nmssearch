#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "output_writer.h"


TEST_CASE( "AlignStats are displayed correctly", "[output]" ) {

    AlignStats a = {
        "query1",
        "subject1",
        95.8,
        200,
        20,
        10,
        1,
        201,
        2,
        202,
        1e-5,
        1024
    };
    AlignStats b = {
        "query2",
        "subject2",
        99.9,
        200,
        20,
        10,
        1,
        201,
        2,
        202,
        1e-5,
        1024
    };

    OutputWriter writer;

    std::string expected1 = "query1\tsubject1\t95.80\t200\t20\t10\t1\t201\t2\t202\t0.00\t1024\n";
    std::string expected2 = "query2\tsubject2\t99.90\t200\t20\t10\t1\t201\t2\t202\t0.00\t1024\n";

    SECTION("Single records are displayed correctly") {
        std::stringstream ss;
        writer.display(a, ss);
        REQUIRE(ss.str() == expected1);
    }

    SECTION("Multiple records are displayed correctly") {
        std::stringstream ss;
        writer.display({a, b}, ss);
        REQUIRE(ss.str() == expected1 + expected2);
    }
}
