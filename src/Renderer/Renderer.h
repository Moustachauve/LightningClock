#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include <const.h>

#define FASTLED_INTERNAL 
#include <FastLED.h>
#include <FastLedCustom.h>


namespace Renderer {
    
    template<typename T_PIXEL_METHOD> class Renderer
    {
    protected:
        T_PIXEL_METHOD* matrix;
        uint32_t millisStep;

        // Modified from WLED
        // https://github.com/Aircoookie/WLED/blob/master/wled00/FX_fcn.cpp 
        CRGB ColToCrgb(RgbwColor color)
        {
            CRGB fastledCol;
            fastledCol.red =   color.R;
            fastledCol.green = color.G;
            fastledCol.blue =  color.B;
            return fastledCol;
        }

    public:
        Renderer(T_PIXEL_METHOD* pMatrix) 
        {
            matrix = pMatrix;
        };
        virtual ~Renderer() {};
        virtual void Draw() = 0;
    };
}