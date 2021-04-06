#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include "Renderer.h"
#include "Solid.h"
#include<Fonts/m3x6.h>

namespace Renderer {
    // 'fish', 13x8px
    const unsigned char fish [] PROGMEM = {
        0x03, 0xc0, 0x87, 0xe0, 0xcf, 0xb0, 0xff, 0xf8, 0xff, 0xf8, 0xcf, 0xf0, 0x87, 0xe0, 0x03, 0xc0
    };

    template<typename T_PIXEL_METHOD> class FishOlu : public Solid<T_PIXEL_METHOD>
    {
    private:
        int16_t fishX = -14;
        int8_t speed = 55;
        unsigned long prevMillis;
    public:
        FishOlu(T_PIXEL_METHOD* pMatrix) : Solid<T_PIXEL_METHOD>(pMatrix) {};
        virtual ~FishOlu() {};
        virtual void Draw()
        {
            Solid<T_PIXEL_METHOD>::Draw();

            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor(RgbwColor(110, 0, 178, 0));
            Renderer<T_PIXEL_METHOD>::matrix->setFont(&m3x6);

            
            int16_t  x1, y1;
            uint16_t w, h;
            const char* text = "OLU";
            Renderer<T_PIXEL_METHOD>::matrix->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
            Renderer<T_PIXEL_METHOD>::matrix->setCursor((MATRIX_WIDTH - w) / 2, 6);

            Renderer<T_PIXEL_METHOD>::matrix->print(text);

            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor(RgbwColor(255, 20, 147, 20));
            Renderer<T_PIXEL_METHOD>::matrix->drawBitmap(fishX, 0, fish, 13, 8, 0);

            unsigned long curMillis = millis();  
            if (curMillis - prevMillis >= speed) {
                prevMillis = curMillis;
                 
                if (fishX++ > MATRIX_WIDTH + 15) {
                    fishX = -14;
                }
            }

            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor();
        };
    };
}