#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h" // Include the wrapper class header
#include <csignal>
#include "sim/in.h"
#include "MessageHandler.h" // Include the MessageHandler header
#include <chrono>
#include <thread>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <mutex>
sim::socket* serverSocket = nullptr; // Global pointer to the server socket
namespace ebikeConstants {
    const std::string CONFIG_PATH = "config/client-config.yaml";
}
// Signal handler function
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nGracefully shutting down..." << std::endl;
        
        if (serverSocket) {
            delete serverSocket;
            serverSocket = nullptr;
        }
        
        std::exit(0); // Exit the program
    }
}



int main2(Poco::JSON::Array::Ptr features, std::mutex& featuresMutex) {
    try {

        // Register the signal handler for SIGINT
        std::signal(SIGINT, signalHandler);

        // Set server IP address
        //sim::set_ipaddr("192.168.1.1");
        std::string serverIp = readConfigValue(ebikeConstants::CONFIG_PATH, "server", "ip");
        std::cout << "Server IP: " << serverIp << std::endl;
        std::string portStr = readConfigValue(ebikeConstants::CONFIG_PATH, "server", "port");
        std::cout << "Server Port: "<< portStr << std::endl;
        int serverPort = std::stoi(portStr);
        sim::set_ipaddr(serverIp.c_str());
       // Create the server socket
        serverSocket = new sim::socket(AF_INET, SOCK_DGRAM, 0);

        // Server binds to a "UDP-like" address
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr); // Bind to localhost
        serverAddr.sin_port = htons(serverPort); // Port 8080
        serverSocket->bind(serverAddr);

        //Poco::JSON::Array::Ptr features = new Poco::JSON::Array;

        std::cout << "Server running and waiting for messages..." << std::endl;

        //wait 5 seconds before stating to receive messages
        std::this_thread::sleep_for(std::chrono::seconds(5));

        while (true) {
            // Buffer to receive data
            char buffer[256];
            struct sockaddr_in clientAddr;

            // Receive data from the client
            ssize_t received = serverSocket->recvfrom(buffer, sizeof(buffer), 0, clientAddr);
            if (received > 0) {
                buffer[received] = '\0'; // Null-terminate the received string

                std::thread([=, &featuresMutex]() {
                    char clientIp[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
                    uint16_t clientPort = ntohs(clientAddr.sin_port);

                    MessageHandler handler;

                    // Lock only around the shared resource
                    const char* response;
                    {
                        std::lock_guard<std::mutex> lock(featuresMutex);
                        response = handler.handleMessage(buffer, clientIp, clientPort, features);
                    }

                    handler.sendResponse(serverSocket, response, clientAddr);

                }).detach();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Server: " << e.what() << std::endl;
    }

    return 0;
}