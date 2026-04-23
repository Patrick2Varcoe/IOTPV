#include "util/MiscUtils.h"
#include "web/WebServer.h"
#include <iostream>
#include "comm/SocketServer.h"
#include "sim/socket.h"
#include <thread>
#include <mutex>
/***TODO: complete code as per assignment specification***/

int main() {
    // DO NOT MODIFY THIS LINE. Creates a reference to a Poco JSON array to store the ebike objects.
    Poco::JSON::Array::Ptr ebikes = new Poco::JSON::Array();

    //** TODO Adjust & add code as per assignment specification ***/
    std::mutex featuresMutex;

    try {
        
        // Using threads to be able to run both socket and web servers simlutaniously
        std::thread udpThread([&]() {
        main2(ebikes, featuresMutex); // Starts the socket server 
        });

        // read webport from yaml file. DO NOT MODIFY THIS LINE, but need to update config/server-config.yaml with your allocated port as per specification.
        int port = std::stoi(readConfigValue("config/server-config.yaml", "webserver", "port"));

        // Create instances of the server class
        WebServer webServer(ebikes);

        // Start the web server 
        webServer.start(port);

        udpThread.join();
    
        

        return 0;
    } catch (const Poco::Exception& ex) {
        std::cerr << "Server error (Poco): " << ex.displayText() << std::endl;
        return 1;
    }

    


    return 0;
}



