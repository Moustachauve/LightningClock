#pragma once

#include <Adafruit_NeoMatrix.h>
#include "Renderer.h"

namespace Renderer {
    
    class Solid : public Renderer
    {
    public:
        Solid(Adafruit_NeoMatrix* pMatrix);
        virtual ~Solid();
        virtual void Draw();
    };
}