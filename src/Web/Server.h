#pragma once

#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <AsyncTCP.h>

#include <const.h>

void OnWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);

namespace Web 
{
    class Server
    {
    protected:
        void SetRoutes();
        
    private:
        /* data */
        AsyncWebServer asyncServer = AsyncWebServer(80);
        AsyncWebSocket asyncWebSocket = AsyncWebSocket("/ws");

    public:
        Server();
        ~Server();
        void Begin();
        void SetText(AsyncWebServerRequest *request);
        void WsCleanupClients();
    };
    
}