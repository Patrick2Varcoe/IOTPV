#ifndef GPSSENSOR_H // Header guard 
#define GPSSENSOR_H

#include <cstdint>
#include "hal/ISensor.h" // Include the ISensor header

class GPSSensor : public ISensor {

public:
    //Constructor to initialize the sensor with an ID 
    GPSSensor(int id) : id(id) {}

    int getId() const override{
        return id;
    }

    int getDimension() const override {
        return 2; 
    }

    virtual std::pair<std::string, std::string> format(std::vector<uint8_t> reading) {
    std::string result(reading.begin(), reading.end());

    size_t commaPos = result.find(',');

    if (commaPos != std::string::npos) {
        std::string first = result.substr(0, commaPos);
        std::string second = result.substr(commaPos + 1);
        return {first, second};}
        
    return {result,result};
    }

private:
    // ID of the sensor
    int id;
    
};
#endif // GPSSENSOR_H