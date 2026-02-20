#ifndef TEMPERATURESENSOR_H  // Header guard 
#define TEMPERATURESENSOR_H 

#include "hal/ISensor.h" // Include the ISensor header
#include <sstream>

class TemperatureSensor : public ISensor {

public:
    //Constructor to initialize the sensor with an ID and dimension
    TemperatureSensor(int id, int dimension) : id(id), dimension(dimension) {}

    //TODO Returns the ID of the sensor
    
    int getId() override {
        return self.id;
    };


    //TODO Returns the dimension (number of measurements) of the sensor
    int getDimension() override {
        return self.dimension;
    };

    //TODO Formats the sensor reading into a string. It needs to convert a vector of bytes to a string.
    virtual std::pair<std::string, std::string> format(std::vector<uint8_t> reading) override { 
           return {
        std::to_string(reading[0]),
        std::to_string(reading[1])
        };
    };

private:
    // ID of the sensor
    int id;

    // Dimension (number of measurements) of the sensor
    int dimension;

};
#endif // TEMPERATURESENSOR_H