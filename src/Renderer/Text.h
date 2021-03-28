#pragma once

#include <Adafruit_NeoMatrix.h>
#include "Renderer.h"
#include<Fonts/m3x6.h>
#include<Fonts/m5x7.h>

namespace Renderer {
    
    class Text : public Renderer
    {
    private:
        const char* text;
        int x = 0;
        int count = 0;
        int countFont = 0;
        int pass = 0;
        
        uint32_t Wheel(byte WheelPos);
    public:
        Text(Adafruit_NeoMatrix* pMatrix);
        virtual ~Text();
        virtual void Draw();
        void setText(const char* pText);
    };
}