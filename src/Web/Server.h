#pragma once

#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <AsyncTCP.h>

#include <const.h>


namespace Web 
{
    class Server
    {
    protected:
        void SetRoutes();
    private:
        /* data */
        AsyncWebServer asyncServer = AsyncWebServer(80);
    public:
        Server();
        ~Server();
        void Begin();
        void SetText(AsyncWebServerRequest *request);
    };
    
}