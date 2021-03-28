#pragma once

#include <Adafruit_NeoMatrix.h>
#include "Text.h"
#include <iostream>

namespace Renderer {
    
    class Clock : public Text
    {
    public:
        Clock(Adafruit_NeoMatrix* pMatrix);
        virtual ~Clock();
        virtual void Draw();
    };
}