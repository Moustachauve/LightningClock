#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include <const.h>

namespace Renderer {
    
    template<typename T_PIXEL_METHOD> class Renderer
    {
    protected:
        T_PIXEL_METHOD* matrix;

    public:
        Renderer(T_PIXEL_METHOD* pMatrix) 
        {
            matrix = pMatrix;
        };
        virtual ~Renderer() {};
        virtual void Draw() = 0;
    };
}