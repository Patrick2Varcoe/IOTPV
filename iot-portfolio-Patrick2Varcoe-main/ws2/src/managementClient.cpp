#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h"
#include "sim/in.h"
#include "util/MiscUtils.h"
/***TODO: complete code as per assignment specification***/

int main(int argc, char* argv[]) {
    if (argc  < 3) {
        std::cerr << "Usage: " << argv[0] << " <socket_client_ip> <socket_server_ip> <json_file> " << std::endl;
        return 1;
    }

    //***IMPORTANT: DO NOT CHANGE THE ORDER, QUANTITY OR TYPE OF THE COMMAND LINE ARGUMENTS*/

    // Get the client IP, eBike ID, CSV file, and number of ports from the command line arguments
    std::string clientIp = argv[1]; // Client socket IP address        
    std::string serverIp = argv[2]; // Server socket IP address
    std::string jsonFile = argv[3]; // JSON file path, normally in the data directory of the project (See examples). JSON file contains the management message to send to the server.

    //TODO: complete code as per assignment specification

    


    std::cout << "Client IP: " << clientIp << std::endl;
    std::cout << "Server IP: " << serverIp << std::endl;
    std::cout << "JSON message file: " << jsonFile << std::endl;


    //  Read JSON file
    std::ifstream file(jsonFile);
    if (!file) {
        std::cerr << "Failed to open JSON file\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonMessage = buffer.str();
    size_t pos = jsonMessage.find("\"timestamp\": \"TODO\"");
    if (pos != std::string::npos) {
        std::string newTimestamp = "\"timestamp\": \"" + getFormattedTime() + "\"";
        jsonMessage.replace(pos, std::string("\"timestamp\": \"TODO\"").length(), newTimestamp);
    }
    //  Setup socket
    sim::set_ipaddr(clientIp.c_str());
    sim::socket client(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in clientAddr{};
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, clientIp.c_str(), &clientAddr.sin_addr);
    clientAddr.sin_port = htons(8090); // any port
    client.bind(clientAddr);

    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(8080);

    //  Send JSON
    ssize_t sent = client.sendto(jsonMessage.c_str(), jsonMessage.size(), 0, serverAddr);

    if (sent > 0) {
        std::cout << "Sent JSON to server:\n" << jsonMessage << std::endl;
    }

    return 0;

}