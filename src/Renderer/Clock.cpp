#include "Clock.h"

namespace Renderer {

    Clock::Clock(Adafruit_NeoMatrix* pMatrix) : Text(pMatrix)
    {
    }

    Clock::~Clock()
    {
    }
    
    void Clock::Draw()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            setText("Fail :(");
        } else {
            char buffer[32];
            strftime(buffer, 32, "%H:%M", &timeinfo);
            setText(buffer);
        }
        
        Text::Draw();
    }
}