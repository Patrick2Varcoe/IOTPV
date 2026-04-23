#define CATCH_CONFIG_MAIN
#include <cstdint>
#include "hal/CSVHALManager.h"
#include "hal/ISensor.h"
#include "catch.hpp"


class MockSensor : public ISensor {
public:
    MockSensor(int id, int dimension) : id(id), dimension(dimension) {}

    int getId() const override { return id; }
    int getDimension() const override { return dimension; }
    std::pair<std::string, std::string> format(std::vector<uint8_t> reading) override { return {"",""}; }

private:
    int id;
    int dimension;
};


TEST_CASE("CSVHALManager functionality", "[CSVHALManager]") {
    CSVHALManager halManager(2);
    auto sensor = std::make_shared<MockSensor>(0, 1);
    halManager.attachDevice(0, sensor);



    SECTION("Attach and release device") {
        halManager.releaseDevice(0);
        REQUIRE_THROWS_AS(halManager.read(0), std::runtime_error);
    }
}