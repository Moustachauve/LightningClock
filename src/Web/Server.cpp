#include "Server.h"
#include <Service/RendererManager.h>

namespace Web 
{
    Server::Server(/* args */)
    {
    }
    
    Server::~Server()
    {
    }

    void Server::SetRoutes()
    {
        Serial.write("Setting Routes\n");
        asyncServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/index.html");
        });

        asyncServer.on("/text", HTTP_GET, std::bind(&Server::SetText, this, std::placeholders::_1));

        asyncServer.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");
    }

    void Server::Begin() 
    {
        Serial.write("Web begin\n");
        SPIFFS.begin(true);
        SetRoutes();
        asyncWebSocket.onEvent(OnWsEvent);
        asyncServer.addHandler(&asyncWebSocket);
        asyncServer.begin();
    }

    void Server::SetText(AsyncWebServerRequest *request)
    {
        if(!request->hasParam("text")) {
            request->send(400, "text/plain", "parameter'?text=' missing");
            return;
        }

        AsyncWebParameter* p = request->getParam("text");

        char* text = new char [p->value().length()+1];
        std::strcpy(text, p->value().c_str());

        Service::RendererManager::Get().getForeground()->setText(text);
        request->send(200, "text/plain", "Done");
    }

    void Server::WsCleanupClients() 
    {
        asyncWebSocket.cleanupClients();
    }
}