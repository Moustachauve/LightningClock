#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <const.h>

#include<Service/NetworkService.h>
#include<Web/Server.h>
#include<Service/RendererManager.h>

#include<Renderer/Solid.h>
#include<Renderer/Heartbeat.h>
#include<Renderer/Text.h>
#include<Renderer/Dna.h>
#include<Renderer/Clock.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

Service::NetworkService networkService;
Web::Server server;

/**Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(27, 8, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRBW           + NEO_KHZ800);
*/

typedef NeoPixelBrightnessBusGfx<NeoGrbwFeature, Neo800KbpsMethod>  NeoPixelBusType;

NeoPixelBusType matrix(MATRIX_WIDTH, MATRIX_HEIGHT, LED_PIN);

// For NeoPixelBus
uint16_t remap(uint16_t x, uint16_t y) {
    return Service::RendererManager::Get().Map(x, y);
}
// For FastLED
uint16_t XY(uint8_t x, uint8_t y) {
    return remap(x, y);
}

Renderer::Renderer<NeoPixelBusType>* renderer;
Renderer::Text<NeoPixelBusType>* textRenderer;

int8_t direction;

void setup()
{
    Serial.begin(115200);
    Serial.println("Booting");
    

    matrix.Begin();
    matrix.Show();
    matrix.setRemapFunction(&remap);
    matrix.setTextWrap(false);
    matrix.SetBrightness(20);
    matrix.fillScreen(DEFAULT_BACKGROUND_COLOR);
    matrix.SetPixelColor(0, RgbwColor(255, 0, 0, 0));
    matrix.SetPixelColor(15, RgbwColor(0,255,0,0));
    matrix.SetPixelColor(16, RgbwColor(0,0,255,0));
    matrix.SetPixelColor(31, RgbwColor(0,0,0,255));
    matrix.setTextColor(DEFAULT_FOREGROUND_COLOR);
    matrix.setFont();
    matrix.setCursor(2, 0);
    matrix.print("Hi:)");
    matrix.Show();

    networkService = Service::NetworkService();
    networkService.Begin();

    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.begin();
    server.Begin();

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    //renderer = new Renderer::Solid<NeoPixelBusType>(&matrix);
    //renderer = new Renderer::Heartbeat<NeoPixelBusType>(&matrix);
    renderer = new Renderer::Dna<NeoPixelBusType>(&matrix);

    //textRenderer = new Renderer::Text<NeoPixelBusType>(&matrix);
    textRenderer = new Renderer::Clock<NeoPixelBusType>(&matrix);

    Service::RendererManager::Get().setBackground(renderer);
    Service::RendererManager::Get().setForeground(textRenderer);

    //textRenderer->setText("Ready");
    textRenderer->setAlignment(Renderer::TextAlignCenter);
    textRenderer->setColor(RgbwColor(0, 0, 0, 255));
    textRenderer->setFont(&m5x7);

    Serial.println("Setup Completed");
}

int count = 0;

void loop()
{
    ArduinoOTA.handle();

    Service::RendererManager::Get().Draw();

    matrix.Show();
}
