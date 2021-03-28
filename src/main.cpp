#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include<Service/NetworkService.h>

#include<Renderer/Solid.h>
#include<Renderer/Text.h>
#include<Renderer/Clock.h>

#define PIN 2
#define NUM_LEDS 216

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

Service::NetworkService networkService;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(27, 8, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRBW           + NEO_KHZ800);


Renderer::Renderer* renderer;
Renderer::Text* textRenderer;

void setup()
{
    Serial.begin(115200);
    Serial.println("Booting");

    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(20);
    matrix.fillScreen(matrix.Color(0, 125, 125));
    matrix.setTextColor(matrix.Color(255, 255, 255));
    matrix.setFont();
    matrix.print("Hi:)");
    matrix.show();

    renderer = new Renderer::Solid(&matrix);
    textRenderer = new Renderer::Text(&matrix);

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

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    Serial.println("Setup Completed");

    textRenderer->setText("Hola");
}

int count = 0;

void loop()
{
    ArduinoOTA.handle();

    //matrix.fillScreen(0);
    renderer->Draw();
    textRenderer->Draw();

    if (count >= 0 && ++count > 25) {
        textRenderer = new Renderer::Clock(&matrix);
        count = -1;
    }

    matrix.show();
    delay(80);
}
