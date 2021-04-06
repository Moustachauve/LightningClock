#pragma once

#include <const.h>
#include <NeoPixelBrightnessBusGfx.h>
#include <Renderer/Text.h>
#include <Renderer/Clock.h>
#include <Renderer/Renderer.h>

namespace Service 
{
    class RendererManager
    {
    private:
        RendererManager();
        ~RendererManager();
        unsigned long nextTime;
        NeoPixelTopology topo = NeoPixelTopology(MATRIX_WIDTH, MATRIX_HEIGHT);

    protected:
        Renderer::Renderer<NeoPixelBusType>* background;
        Renderer::Text<NeoPixelBusType>* foreground;
        
    public:
        static RendererManager& Get();

        CRGB leds[LED_COUNT];

        Renderer::Renderer<NeoPixelBusType>* getBackground();
        void setBackground(Renderer::Renderer<NeoPixelBusType>* background);
        
        Renderer::Text<NeoPixelBusType>* getForeground();
        void setForeground(Renderer::Text<NeoPixelBusType>* foreground);

        void Draw();
        uint16_t Map(uint16_t x, uint16_t y);
        uint32_t crgb_to_col(CRGB fastled);
    };
}
