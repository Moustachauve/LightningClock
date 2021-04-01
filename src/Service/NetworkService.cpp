#include "NetworkService.h"

namespace Service 
{
    void NetworkService::Begin()
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        while (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            Serial.println("Connection Failed! Rebooting...");
            delay(5000);
            ESP.restart();
        }

        Serial.println("Network Ready");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }

    NetworkService::NetworkService()
    {
    }

    NetworkService::~NetworkService()
    {
    }
}