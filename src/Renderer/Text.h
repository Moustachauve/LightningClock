#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include "Renderer.h"
#include<Fonts/m3x6.h>
#include<Fonts/m5x7.h>

namespace Renderer {
    
    template<typename T_PIXEL_METHOD> class Text : public Renderer<T_PIXEL_METHOD>
    {
    private:
        const char* text;
        int x = 0;
        int count = 0;
        int countFont = 0;
        int pass = 0;
        
        // Input a value 0 to 255 to get a color value.
        // The colours are a transition r - g - b - back to r.
        uint32_t Wheel(byte WheelPos) 
        {
            WheelPos = 255 - WheelPos;
            if(WheelPos < 85) {
                return Renderer<T_PIXEL_METHOD>::matrix->Color(255 - WheelPos * 3, 0, WheelPos * 3);
            }
            if(WheelPos < 170) {
                WheelPos -= 85;
                return Renderer<T_PIXEL_METHOD>::matrix->Color(0, WheelPos * 3, 255 - WheelPos * 3);
            }
            WheelPos -= 170;
            return Renderer<T_PIXEL_METHOD>::matrix->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
        };
    public:
        Text(T_PIXEL_METHOD* pMatrix) : Renderer<T_PIXEL_METHOD>(pMatrix) {};
        virtual ~Text() {};
        virtual void Draw()
        {    
            if (++count >= 255) {
                count = 0;
                if (++countFont > 2) {
                    countFont = 0;
                }
            }

            switch (countFont)
            {
                case 0:
                    Renderer<T_PIXEL_METHOD>::matrix->setFont();
                    //matrix.print(F("a"));
                    break;
                case 1:
                    Renderer<T_PIXEL_METHOD>::matrix->setFont(&m5x7);
                    //matrix.print(F("b"));
                    break;
                case 2:
                    Renderer<T_PIXEL_METHOD>::matrix->setFont(&m3x6);
                    //matrix.print(F("c"));
            }

            /*
            if (countFont == 0) {
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, 0);
            } else {
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, 6);
            }
            */

            // For centering
            int16_t  x1, y1;
            uint16_t w, h;
            Renderer<T_PIXEL_METHOD>::matrix->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
            uint16_t leftBorder = (MATRIX_WIDTH - w) / 2;
            Renderer<T_PIXEL_METHOD>::matrix->setCursor(leftBorder, countFont == 0 ? 0 : 6);

            Renderer<T_PIXEL_METHOD>::matrix->print(text);


            // For scrolling text
            /*if(--x < -46) {
                x = matrix.width();
            }*/


            Renderer<T_PIXEL_METHOD>::matrix->setTextColor(Wheel((count) & 255));
        };
        void setText(const char* pText) 
        {
            text = pText;
        };
    };
}