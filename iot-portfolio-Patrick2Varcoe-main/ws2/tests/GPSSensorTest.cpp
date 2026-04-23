#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstdint>
#include "sensor/GPSSensor.h"
#include <vector>
#include <cstring>
#include <sstream>

TEST_CASE("GPSSensor functionality", "[GPSSensor]") {
    
    
    auto gsensor = std::make_shared<GPSSensor>(0);

    SECTION("Check ID and dimension") {

        REQUIRE(gsensor->getId() == 0);
        REQUIRE(gsensor->getDimension() == 2);
    }


        SECTION("Format reading") {

        std::string input = "51.457130,-2.557153,0.123,0.456,9.789";
        std::vector<uint8_t> testData(input.begin(), input.end());

        auto formatted = gsensor->format(testData);

        // Split position (lat, lon)
        std::stringstream posStream(formatted.first);
        std::string lat, lon;
        std::getline(posStream, lat, ',');
        std::getline(posStream, lon, ',');

        // Split acceleration (x, y, z)
        std::stringstream accStream(formatted.second);
        std::string acc_x, acc_y, acc_z;
        std::getline(accStream, acc_x, ',');
        std::getline(accStream, acc_y, ',');
        std::getline(accStream, acc_z, ',');

        REQUIRE(lat == "51.457130");
        REQUIRE(lon == "-2.557153");
        REQUIRE(acc_x == "0.123");
        REQUIRE(acc_y == "0.456");
        REQUIRE(acc_z == "9.789");
    }
}