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
            request->send(200, "text/plain", "Hello, world");
        });

        asyncServer.on("/text", HTTP_GET, std::bind(&Server::SetText, this, std::placeholders::_1));
    }

    void Server::Begin() 
    {
        Serial.write("Web begin\n");
        SetRoutes();
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
}