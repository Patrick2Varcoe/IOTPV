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
#include "sim/socket.h" 
#include "sim/in.h" 
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
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

    // Set client IP address
    
    const char* myIp =  clientIp.c_str();
    sim::set_ipaddr(myIp);
    
    // Create the client socket
    sim::socket client(AF_INET, SOCK_DGRAM, 0);


    // Define the server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.1", &serverAddr.sin_addr); // Server is on localhost
    serverAddr.sin_port = htons(8080); // Port 8080
        
     // Bind the client socket to the client address
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, myIp, &clientAddr.sin_addr);
    clientAddr.sin_port = htons(8085); // Let the OS choose a random port
    client.bind(clientAddr);


    // Create Hal Manager
    CSVHALManager Manager1(numPorts);

    // Create GPS Sensor
    auto Gsensor = std::make_shared<GPSSensor>(0);

    // Attatch GPS Sensor to Hal Manager
    int desPort = stoi(ebikeId);
    Manager1.attachDevice(desPort, Gsensor);


    // Initialize Manager with CSV File
    Manager1.initialise(csvFile);

    for (int i = 0; i < maxReadings; i++) {

        string dateandtime = getFormattedTime();
        auto raw = Manager1.read(desPort);
        auto formatted = Gsensor->format(raw);

        string msg = "[EBCLIENT] " + dateandtime + " gps: lat: " + formatted.first + " lon: " + formatted.second + "(unlocked)";
        //std::cout << msg ;

        string JSString = generateJsonString({{"lat", formatted.first},{"lon" formatted.second}});
        std::cout << JSString;

        json feature;

        feature["type"] = "Feature";
        feature["geometry"] = {{"type","Point"},{"coordinates",{formatted}}};
        feature["properties"]= {{"name", "EBike"}};

        // Message to send
        char* message = const_cast<char*>(msg.c_str());
        ssize_t sent = client.sendto(message, strlen(message), 0, serverAddr);
        if (sent > 0) {
            std::cout << "Message to server: " << message << std::endl;
        }
        //std::cout <<"[EBCLIENT] " << dateandtime << " gps: "<<"lat: " <<formatted.first << " lon: " << formatted.second << "(unlocked)"<< std::endl;
                // Buffer to receive response
        char buffer[256];
        struct sockaddr_in fromAddr;
        
        // Receive response from the server
        ssize_t received = client.recvfrom(buffer, sizeof(buffer), 0, fromAddr);

        //wait 5 seconds before closing
        std::this_thread::sleep_for(std::chrono::seconds(5));

        if (received > 0) {
            buffer[received] = '\0'; // Null-terminate the received string

            // Print the received response
            char fromIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &fromAddr.sin_addr, fromIp, sizeof(fromIp));
            uint16_t fromPort = ntohs(fromAddr.sin_port);

            std::cout << "Received response: " << buffer << std::endl;
            std::cout << "From: " << fromIp << ":" << fromPort << std::endl;
        }
        //std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    // Release Device from Manager
    Manager1.releaseDevice(desPort);
    

    std::cout <<"[EBCLIENT] Shutting down \n";

    return 0;
}