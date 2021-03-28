#pragma once

#include <Adafruit_NeoMatrix.h>

namespace Renderer {
    
    class Renderer
    {
    protected:
        Adafruit_NeoMatrix* matrix;

    public:
        Renderer(Adafruit_NeoMatrix* pMatrix);
        virtual ~Renderer();
        virtual void Draw() = 0;
    };
}