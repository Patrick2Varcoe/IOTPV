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

// Class to handle UDP messages for the UDP server
// Parses messages and processes them based upon type of message e.g. management command
// Handles appropriate responses to client
class MessageHandler {
    private:
        //Deafult value for data interval
        int BaseDataInterval = 5;
        //Response buffer to safely handle correct return type 
        std::string responseBuffer;
    public:

    const char* handleMessage(const char* message, const char* clientIp, uint16_t clientPort, Poco::JSON::Array::Ptr features) {
        // Process incoming message and provide relevant response
        // Params : Raw message from client, Ip of client, Port of client, EBike list

        std::string Smessage = message;

        // Block to check for and handle Management client messages
        if (Smessage[0] == '{') {
            Poco::JSON::Parser parser;
            auto result = parser.parse(Smessage);
            Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();
            // This section handles management messages, without directive is not management message
            if (obj->has("directive")){
            
            //Parse the directive from the message
            std::string directive = obj->getValue<std::string>("directive");
            
            // Lock / Unlock Management Command
            if (directive == "COMMAND") {

                // Get IDs array for Ebikes
                Poco::JSON::Array::Ptr ids = obj->getArray("ebike_ids");

                // Get action from message
                Poco::JSON::Array::Ptr actions = obj->getArray("action");
                std::string action = actions->getElement<std::string>(0);

                // Apply action to all relevant Ebikes
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
                // Return response to inform sender
                return "command processed";
            }
        
        
    
            // Management message to update data interval
            else if (directive == "SETUP"){
                if (!obj->has("data_interval")) {
                    return "invalid setup";
                }
                // Parse new data interval
                int newInterval = obj->getValue<int>("data_interval");
                //Update global default for new clients
                BaseDataInterval = newInterval;
                std::cout << "Setting global data interval to: " << newInterval << "\n";

                // Apply data interval to exsisting bike objects
                for (size_t i = 0; i < features->size(); i++) {
                    auto feature = features->getObject(i);
                    auto props = feature->getObject("properties");

                    props->set("data_interval", newInterval);
                }
                // Response to inform sender update has been preformed
                responseBuffer = "data_interval: " + std::to_string(newInterval);
                return(responseBuffer.c_str());
                }}
        }
        // Section for initial setup of new ebike
        if (Smessage.substr(0,9) == "ebike_id:"){

            // Find position of relevant elements
            size_t idPos = Smessage.find("ebike_id:");
            size_t timestampPos = Smessage.find("timestamp:");
            // Parse the ebikes ID from message
            std::string idStr = Smessage.substr(idPos + 9, timestampPos - (idPos + 9));

            int eId = std::stoi(idStr);

            // Build initial JSON object with geometries
            Poco::JSON::Object::Ptr props = new Poco::JSON::Object;
            props->set("ID", eId);
            props->set("type", "config");
            props->set("status", "unlocked"); // Default Status
            
            Poco::JSON::Object::Ptr feature = new Poco::JSON::Object;
            feature->set("type", "Feature");
            feature->set("properties", props);

            features->add(feature);
            std::cout << "Handling message from " << clientIp << ":" << clientPort << " - " << message << std::endl;

            // Respond to client informing the requested data interval from server
            responseBuffer = "status: success data_interval: " + std::to_string(BaseDataInterval);
            return(responseBuffer.c_str());

        }


        // Normal client transmissions 
        else {
            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(Smessage);
            Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();
            // Parse Ebike information
            int eId = std::stoi(obj->getValue<std::string>("id"));
            std::string datetime = obj->getValue<std::string>("timestamp");
            // Parse GPS 
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