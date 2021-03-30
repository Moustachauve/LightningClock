#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include "Text.h"
#include <iostream>

namespace Renderer {
    
    template<typename T_PIXEL_METHOD> class Clock : public Text<T_PIXEL_METHOD>
    {
    public:
        Clock(T_PIXEL_METHOD* pMatrix) : Text<T_PIXEL_METHOD>(pMatrix) {};
        virtual ~Clock() {};
        virtual void Draw() 
        {
            struct tm timeinfo;
            if (!getLocalTime(&timeinfo)) {
                this->setText("Fail :(");
            } else {
                char buffer[32];
                strftime(buffer, 32, "%H:%M", &timeinfo);
                this->setText(buffer);
            }
            
            Text<T_PIXEL_METHOD>::Draw();
        };
    };
}