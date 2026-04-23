#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H


#include <iostream>
#include <sstream>


#include <iostream>
#include <cstring>
//#include <arpa/inet.h>
#include "sim/socket.h"
#include <string>

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>


class MessageHandler {
    private:
        int BaseDataInterval = 5;
        std::string responseBuffer;
    public:

    const char* handleMessage(const char* message, const char* clientIp, uint16_t clientPort, Poco::JSON::Array::Ptr features) {
        // Handle the message (existing implementation)


        std::string Smessage = message;

        if (Smessage[0] == '{') {
            Poco::JSON::Parser parser;
            auto result = parser.parse(Smessage);
            Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();
            
            if (obj->has("directive")){

            std::string directive = obj->getValue<std::string>("directive");

            if (directive == "COMMAND") {

                // Get IDs array
                Poco::JSON::Array::Ptr ids = obj->getArray("ebike_ids");

                // Get action array (take first element)
                Poco::JSON::Array::Ptr actions = obj->getArray("action");
                std::string action = actions->getElement<std::string>(0);

                for (size_t i = 0; i < ids->size(); i++) {
                    int targetId = ids->getElement<int>(i);

                    for (size_t j = 0; j < features->size(); j++) {
                        auto feature = features->getObject(j);
                        auto props = feature->getObject("properties");

                        if (props->getValue<int>("ID") == targetId) {
                            props->set("status", action);
                            std::cout << "Updated eBike " << targetId << " to " << action << "\n";
                        }
                    }
                }

                return "command processed";
            }
        
        
    
            
            else if (directive == "SETUP"){
                if (!obj->has("data_interval")) {
                    return "invalid setup";
                }

                int newInterval = obj->getValue<int>("data_interval");
                BaseDataInterval = newInterval;
                std::cout << "Setting global data interval to: " << newInterval << "\n";


                for (size_t i = 0; i < features->size(); i++) {
                    auto feature = features->getObject(i);
                    auto props = feature->getObject("properties");

                    props->set("data_interval", newInterval);
                }
                responseBuffer = "data_interval: " + std::to_string(newInterval);
                return(responseBuffer.c_str());
                }}
        }

        if (Smessage.substr(0,9) == "ebike_id:"){
            size_t idPos = Smessage.find("ebike_id:");
            size_t timestampPos = Smessage.find("timestamp:");

            std::string idStr = Smessage.substr(idPos + 9, timestampPos - (idPos + 9));

            int eId = std::stoi(idStr);

            // Build JSON object (similar to your GeoJSON but simpler)
            Poco::JSON::Object::Ptr props = new Poco::JSON::Object;
            props->set("ID", eId);
            props->set("type", "config");
            props->set("status", "unlocked"); // Default Value
            // Option 1: push directly to features (if you want everything in one list)
            Poco::JSON::Object::Ptr feature = new Poco::JSON::Object;
            feature->set("type", "Feature");
            feature->set("properties", props);

            features->add(feature);
            std::cout << "Handling message from " << clientIp << ":" << clientPort << " - " << message << std::endl;
            responseBuffer = "status: success data_interval: " + std::to_string(BaseDataInterval);
            return(responseBuffer.c_str());

        }

        else {
            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(Smessage);
            Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();

            int eId = std::stoi(obj->getValue<std::string>("id"));
            std::string datetime = obj->getValue<std::string>("timestamp");

            Poco::JSON::Object::Ptr gps = obj->getObject("gps");
            double lat = gps->getValue<double>("lat");
            double lon = gps->getValue<double>("lon");

            std::string status = obj->getValue<std::string>("status");

            // Build GeoJSON (same as before)
            Poco::JSON::Object::Ptr geometry = new Poco::JSON::Object;
            geometry->set("type", "Point");

            Poco::JSON::Array::Ptr coords = new Poco::JSON::Array;
            coords->add(lon);
            coords->add(lat);

            geometry->set("coordinates", coords);

            Poco::JSON::Object::Ptr props = new Poco::JSON::Object;
            props->set("name", "EBike");
            props->set("ID", eId);
            props->set("Time", datetime);
            props->set("status", status);

            Poco::JSON::Object::Ptr feature = new Poco::JSON::Object;
            feature->set("type", "Feature");
            feature->set("geometry", geometry);
            feature->set("properties", props);

            features->add(feature);
        }
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