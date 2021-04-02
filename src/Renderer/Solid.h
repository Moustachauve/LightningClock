#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include "Renderer.h"

namespace Renderer {
    
    template<typename T_PIXEL_METHOD> class Solid : public Renderer<T_PIXEL_METHOD>
    {
    protected:
        RgbwColor color = HtmlColor(DEFAULT_BACKGROUND_COLOR);
    public:
        Solid(T_PIXEL_METHOD* pMatrix) : Renderer<T_PIXEL_METHOD>(pMatrix) {};
        virtual ~Solid() {};
        virtual void Draw()
        {
            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor(color);
            Renderer<T_PIXEL_METHOD>::matrix->fillScreen(0);
            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor();
        };

        RgbwColor GetColor()
        {
            return color;
        }

        void SetColor(RgbwColor pColor) 
        {
            color = pColor;
        }
    };
}