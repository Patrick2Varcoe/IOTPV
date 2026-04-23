#ifndef GPSSENSOR_H // Header guard 
#define GPSSENSOR_H

#include <cstdint>
#include "hal/ISensor.h" // Include the ISensor header
#include <sstream>
class GPSSensor : public ISensor {

public:
    //Constructor to initialize the sensor with an ID 
    GPSSensor(int id) : id(id) {}

    int getId() const override{
        return id;
    }

    int getDimension() const override {
        return 2; //Updated for new dimensions
    }

    virtual std::pair<std::string, std::string> format(std::vector<uint8_t> reading) {
    std::string result(reading.begin(), reading.end());

    std::cout << result << "   Raw String" << std::endl;
    std::stringstream ss(result);
    std::string item;
    std::vector<std::string> values;

    while (std::getline(ss, item, ',')) {
        values.push_back(item);
    }

    // If we only got 1 value, try semicolon
    if (values.size() == 1) {
        values.clear();
        std::stringstream ss2(result);
        while (std::getline(ss2, item, ';')) {
            values.push_back(item);
        }
    }
    // Return two sets of value, (lat and lon) and all the accellerations
    if (true){//(values.size() >= 5) {
        return {
            values[0] + "," + values[1],                      // lat,lon
            values[2] + "," + values[3] + "," + values[4]     // acc
        };
    }
    //Fallback
    std::cout << "DEBUG : FALLBACK RETURN";
    return {result,result};}

private:
    // ID of the sensor
    int id;
    
};
#endif // GPSSENSOR_H