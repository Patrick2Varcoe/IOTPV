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

        std::vector<uint8_t> testData;

        double lat = 51.457130;
        double lon = -2.557153;

        testData.resize(sizeof(lat) + sizeof(lon));

        std::memcpy(testData.data(), &lat, sizeof(lat));
        std::memcpy(testData.data() + sizeof(lat), &lon, sizeof(lon));
        auto formatted = gsensor->format(testData);
        REQUIRE(formatted.first == "lat:51.457130") ;
        REQUIRE(formatted.second == "lon:-2.557153");

    }
}