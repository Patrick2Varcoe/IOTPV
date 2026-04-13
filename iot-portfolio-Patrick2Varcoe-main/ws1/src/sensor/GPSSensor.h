#ifndef GPSSENSOR_H // Header guard 
#define GPSSENSOR_H

#include "hal/ISensor.h" // Include the ISensor header

class GPSSensor : public ISensor {

public:
    //Constructor to initialize the sensor with an ID 
    TemperatureSensor(int id) : id(id) {}

    int getId() const override{
        return id;
    }

    virtual std::pair<std::string, std::string> format(std::vector<uint8_t> reading) {
        std::string result(reading.begin(), reading.end());
        return {result,result};
    }

private:
    // ID of the sensor
    int id;
}
#endif // GPSSENSOR_H