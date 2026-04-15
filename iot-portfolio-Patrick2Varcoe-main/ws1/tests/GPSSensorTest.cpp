#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstdint>
#include "sensor/GPSSensor.h"
#include <vector>
#include <cstring>

TEST_CASE("GPSSensor functionality", "[GPSSensor]") {
    
    
    auto gsensor = std::make_shared<GPSSensor>(0);

    SECTION("Check ID and dimension") {

        REQUIRE(gsensor->getId() == 0);
        REQUIRE(gsensor->getDimension() == 2);
    }


        SECTION("Format reading") {

        std::string input = "51.457130;-2.557153";
        std::vector<uint8_t> testData(input.begin(), input.end());
        auto formatted = gsensor->format(testData);
        REQUIRE(formatted.first == "51.457130") ;
        REQUIRE(formatted.second == "-2.557153");

    }
}