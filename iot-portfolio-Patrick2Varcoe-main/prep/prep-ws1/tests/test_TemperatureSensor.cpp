#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TemperatureSensor.h"

TEST_CASE("TemperatureSensor functionality", "[TemperatureSensor]") {
    
    //TODO: Create a TemperatureSensor object with ID 0 and dimension 1
    auto tsensor = std::make_shared<TemperatureSensor>(0,1);

    SECTION("Check ID and dimension") {

        REQUIRE(tsensor.getId() == 0);
        REQUIRE(tsensor.getDimension() == 1);
    }
    // TODO: Add assertions to check ID is 0 and dimension is 1.
    


    SECTION("Format reading") {
        testData = {50,55,46,53,50};
        auto formatted = tsensor->format(testData);
        REQUIRE(formatted[0] == "Celsius") ;
        REQUIRE(formatted[1] == "27.52")
           // TODO: Create a reading vector {50,55,46,53,50} ("27.52" in ASCII).
        // TODO: Call format(...) and assert the unit is "Celsius" and the value is "27.52".
    
    }
}