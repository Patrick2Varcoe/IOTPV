#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H


#include <iostream>
#include <sstream>


#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h"
#include <string>

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>


class MessageHandler {
    public:

    const char* handleMessage(const char* message, const char* clientIp, uint16_t clientPort) {
        // Handle the message (existing implementation)


        std::string Smessage = message;

        size_t bracketEnd = Smessage.find("] ");
        size_t gpsPos = Smessage.find(" gps:");

        std::string datetime = Smessage.substr(
            bracketEnd + 6,
            gpsPos - (bracketEnd + 2)
        );

        size_t idPos = Smessage.find("[EBCLIENT]:");
        int eId = std::stoi(Smessage.substr(idPos + 11, idPos + 12));
        size_t latPos = Smessage.find("lat:");
        size_t lonPos = Smessage.find("lon:");
        size_t statusStart = Smessage.find("(", lonPos);
        size_t statusEnd = Smessage.find(")", statusStart);

        double lat = std::stod(Smessage.substr(latPos + 4, lonPos - (latPos + 4)));
        double lon = std::stod(Smessage.substr(lonPos + 4, statusStart - (lonPos + 4)));
        std::string status = Smessage.substr(statusStart + 1, statusEnd - statusStart - 1);

        Poco::JSON::Object::Ptr geometry = new Poco::JSON::Object;
        geometry->set("type", "Point");

        Poco::JSON::Array::Ptr coords = new Poco::JSON::Array;
        coords->add(lat);
        coords->add(lon);

        geometry->set("coordinates", coords);

        Poco::JSON::Object::Ptr props = new Poco::JSON::Object;
        props->set("name", "EBike");
        props->set("ID",eId);
        props->set("Time",datetime);


        Poco::JSON::Object::Ptr feature = new Poco::JSON::Object;
        feature->set("type", "Feature");
        feature->set("geometry", geometry);
        feature->set("properties", props);


        // TODO
        // parses the data into a JSON object, converts it to a GeoJSON object and pushes it to the eBikes list

        std::cout << "Handling message from " << clientIp << ":" << clientPort << " - " << message << std::endl;

        return "OK";
    }

    void sendResponse(sim::socket* serverSocket, const char* response, const struct sockaddr_in& clientAddr) {
        ssize_t sent = serverSocket->sendto(response, strlen(response), 0, clientAddr);

        // Print the response sent to the client and clientAddr
        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
        uint16_t clientPort = ntohs(clientAddr.sin_port);

        std::cout << "Server: " << response << " sent to client: " << clientIp << ":" << clientPort << std::endl;

        if (sent > 0) {
            std::cout << "Response sent to client: " << response << std::endl;
        }
    }
};

#endif 