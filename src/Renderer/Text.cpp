#include "Text.h"

namespace Renderer {

    Text::Text(Adafruit_NeoMatrix* pMatrix) : Renderer(pMatrix)
    {
    }

    Text::~Text()
    {
    }
    
    void Text::Draw()
    {
        if (countFont == 0) {
            matrix->setCursor(x, 0);
        } else {
            matrix->setCursor(x, 6);
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
                matrix->setFont();
                //matrix->print(F("a"));
                break;
            case 1:
                matrix->setFont(&m5x7);
                //matrix->print(F("b"));
                break;
            case 2:
                matrix->setFont(&m3x6);
                //matrix->print(F("c"));
        }

        matrix->print(text);

        // For scrolling text
        
        if(--x < -46) {
            x = matrix->width();
        }

        matrix->setTextColor(Wheel((count) & 255));

    }

    void Text::setText(const char* pText)
    {
        text = pText;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Text::Wheel(byte WheelPos) 
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85) {
            return matrix->Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        if(WheelPos < 170) {
            WheelPos -= 85;
            return matrix->Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        WheelPos -= 170;
        return matrix->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}