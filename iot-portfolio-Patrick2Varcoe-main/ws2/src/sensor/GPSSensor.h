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
        return 5; //Updated for new dimensions
    }

    virtual std::pair<std::string, std::string> format(std::vector<uint8_t> reading) {
    std::string result(reading.begin(), reading.end());


    std::stringstream ss(result);
    std::string item;
    std::vector<std::string> values;

    while (std::getline(ss, item, ',')) {
        values.push_back(item);
    }
    // Return two sets of value, (lat and lon) and all the accellerations
    if (values.size() >= 5) {
        std::string pos = values[0] + "," + values[1];
        std::string acc = values[2] + "," + values[3] + "," + values[4];
        return {pos, acc};
    }
    //Fallback
    return {result,result};}

private:
    // ID of the sensor
    int id;
    
};
#endif // GPSSENSOR_H