#include <string>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>
#include "util/MiscUtils.h"
#include "hal/CSVHALManager.h"
#include "sensor/GPSSensor.h"
#include <ctime>
#include <bits/stdc++.h>

using namespace std;
/***TODO: complete code as per assignment specification***/

// --- Constants ---
namespace ebikeConstants {
    const std::string CONFIG_PATH = "config/client-config.yaml";
}


string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <client_ip> <ebike_id> <csv_file> <num_ports>" << std::endl;
        return 1;
    }
    
    //***IMPORTANT: DO NOT CHANGE THE ORDER, QUANTITY OR TYPE OF THE COMMAND LINE ARGUMENTS*/
    
    // Get the client IP, eBike ID, CSV file, and number of ports from the command line arguments
    std::string clientIp = argv[1]; // Client IP address
    std::string ebikeId = argv[2]; // eBike ID
    std::string csvFile = argv[3]; // CSV file path, normally in the data directory of the project
    int numPorts = std::atoi(argv[4]); //Number of ports of the HAL manager

    int maxReadings = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "client", "max_readings"));
   

    std::cout << "Client IP: " << clientIp << std::endl;
    std::cout << "eBike ID: " << ebikeId << std::endl;
    std::cout << "CSV File: " << csvFile << std::endl;
    std::cout << "Number of Ports: " << numPorts << std::endl;
    std::cout << "Max Readings: " << maxReadings << std::endl;

    // Create Hal Manager
    CSVHALManager Manager1(numPorts);

    // Create GPS Sensor
    auto Gsensor = std::make_shared<GPSSensor>(0);

    // Attatch GPS Sensor to Hal Manager
    Manager1.attachDevice(2, Gsensor);


    // Initialize Manager with CSV File
    Manager1.initialise(csvFile);

    /*
    char arrDate[12];
    string strDate = "";
    char arrTime[12];
    string strTime = "";
    string dateandtime = "";
    */
    for (int i = 0; i < maxReadings; i++) {


        /*
        time_t timestamp = time(NULL);
        struct tm datetime = *localtime(&timestamp);
        strftime(arrDate, 12, "%m/%d/%y", &datetime);
        strftime(arrTime, 12,"%I:%M:%S", &datetime);
        strDate = convertToString(arrDate,12);
        strTime = convertToString(arrTime,12);
        dateandtime = strDate + strTime;

        */
        time_t timestamp = time(NULL);
        string dateandtime = getFormattedTime();
        auto raw = Manager1.read(2);
        auto formatted = Gsensor->format(raw);

        std::cout <<"[EBCLIENT] " << dateandtime << " gps: "<<"lat: " <<formatted.first << " lon: " << formatted.second << "(unlocked)"<< std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    // Release Device from Manager
    Manager1.releaseDevice(2);
    

    std::cout <<"EBCLIENT] Shutting down";

     //TODO: complete code as per assignment specification
    return 0;
}