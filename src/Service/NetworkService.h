#pragma once

#include <WiFi.h>

namespace Service {
    
    class NetworkService
    {
    private:
        const char *ssid = "Connecting...";
        const char *password = "Wololo42";

    public:
        NetworkService();
        ~NetworkService();
        void Begin();
    };
    
    
}