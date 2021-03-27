#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include<Fonts/m3x6.h>
#include<Fonts/m5x7.h>

const char *ssid = "Connecting...";
const char *password = "Wololo42";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;


#define PIN 2
#define NUM_LEDS 216

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(27, 8, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRBW           + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Booting");

    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(20);
    matrix.setTextColor(colors[0]);
    matrix.setFont();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
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

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

int count = 0;
int countFont = 0;
int pass = 0;

void loop()
{
    ArduinoOTA.handle();

    matrix.fillScreen(0);
    if (countFont == 0) {
        matrix.setCursor(0, 0);
    } else {
        matrix.setCursor(0, 6);
    }

    if (++count >= 255) {
        count = 0;
        if (++countFont > 2) {
            countFont = 0;
        }
    }

    switch (countFont)
    {
        case 0:
            matrix.setFont();
            matrix.print(F("a"));
            break;
        case 1:
            matrix.setFont(&m5x7);
            matrix.print(F("b"));
            break;
        case 2:
            matrix.setFont(&m3x6);
            matrix.print(F("c"));
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        matrix.print(F("Fail :("));
    } else {
        matrix.print(&timeinfo, "%H");
        matrix.print(F(":"));
        matrix.print(&timeinfo, "%M");
    }
    
    //matrix.setTextColor(colors[pass]);
    matrix.setTextColor(Wheel((count) & 255));
    matrix.show();
    delay(10);
}
