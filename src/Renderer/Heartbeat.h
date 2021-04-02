#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include "Solid.h"

namespace Renderer {
    
    // Adapted heartbeat effect from WLED 
    // https://github.com/Aircoookie/WLED/blob/master/wled00/FX.cpp
    template<typename T_PIXEL_METHOD> class Heartbeat : public Solid<T_PIXEL_METHOD>
    {
    protected:
        uint8_t speed = 128;
        uint8_t intensity = 128;
        uint16_t brightness = 128;
        bool isSecondBeat = false;

    public:
        Heartbeat(T_PIXEL_METHOD* pMatrix) : Solid<T_PIXEL_METHOD>(pMatrix) {};
        virtual ~Heartbeat() {};
        virtual void Draw()
        {
            uint8_t bpm = 40 + (speed >> 4);
            uint32_t msPerBeat = (60000 / bpm);
            uint32_t secondBeat = (msPerBeat / 3);

            uint32_t bri_lower = brightness;
            bri_lower = bri_lower * 2042 / (2048 + intensity);
            brightness = bri_lower;

            unsigned long beatTimer = millis() - Renderer<T_PIXEL_METHOD>::millisStep;
            if((beatTimer > secondBeat) && !isSecondBeat) { // time for the second beat?
                brightness = UINT16_MAX; //full bri
                isSecondBeat = true;
            }
            if(beatTimer > msPerBeat) { // time to reset the beat timer?
                brightness = UINT16_MAX; //full bri
                isSecondBeat = false;
                Renderer<T_PIXEL_METHOD>::millisStep = millis();
            }

            for (uint16_t i = 0; i < LED_COUNT; i++) {
                CRGB crgbColor = Renderer<T_PIXEL_METHOD>::ColToCrgb(Solid<T_PIXEL_METHOD>::color);
                crgbColor.nscale8_video(brightness >> 8);
                Renderer<T_PIXEL_METHOD>::matrix->SetPixelColor(i, RgbColor(crgbColor.r, crgbColor.g, crgbColor.b));
                //setPixelColor(i, color_blend(color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), 255 - (SEGENV.aux1 >> 8)));
            }
        };
    };
}