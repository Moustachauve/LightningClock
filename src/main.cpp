#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <const.h>

#include<Service/NetworkService.h>

#include<Renderer/Solid.h>
#include<Renderer/Text.h>
#include<Renderer/Clock.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

Service::NetworkService networkService;

/**Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(27, 8, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRBW           + NEO_KHZ800);
*/

typedef NeoPixelBrightnessBusGfx<NeoGrbwFeature, Neo800KbpsMethod>  NeoPixelBusType;

NeoPixelBusType matrix(MATRIX_WIDTH, MATRIX_HEIGHT, LED_PIN);
NeoTopology<RowMajorAlternating270Layout> topo(MATRIX_WIDTH, MATRIX_HEIGHT);

uint16_t remap(uint16_t x, uint16_t y) {
    return topo.Map(x, y);
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
    matrix.fillScreen(matrix.Color(0, 125, 125));
    matrix.SetPixelColor(0, RgbwColor(255, 0, 0, 0));
    matrix.SetPixelColor(15, RgbwColor(0,255,0,0));
    matrix.SetPixelColor(16, RgbwColor(0,0,255,0));
    matrix.SetPixelColor(31, RgbwColor(0,0,0,255));
    matrix.setTextColor(matrix.Color(255, 255, 255));
    matrix.setFont();
    matrix.setCursor(2, 0);
    matrix.print("Hi:)");
    matrix.Show();

    renderer = new Renderer::Solid<NeoPixelBusType>(&matrix);
    textRenderer = new Renderer::Text<NeoPixelBusType>(&matrix);


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
    textRenderer = new Renderer::Clock<NeoPixelBusType>(&matrix);
    textRenderer->setAlignment(Renderer::TextAlignLeft);

    Serial.println("Setup Completed");
}

int count = 0;

void loop()
{
    ArduinoOTA.handle();

    renderer->Draw();
    textRenderer->Draw();

    if (count >= 0 && ++count > 200) {
        textRenderer->setFont(&m5x7);
        textRenderer->setAlignment(Renderer::TextAlignMarqueeBounce);
        count = -1;
    }

    matrix.Show();
}
