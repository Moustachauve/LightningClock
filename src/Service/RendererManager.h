#pragma once

#include <const.h>
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

    protected:
        Renderer::Renderer<NeoPixelBusType>* background;
        Renderer::Text<NeoPixelBusType>* foreground;
        
    public:
        static RendererManager& Get();

        Renderer::Renderer<NeoPixelBusType>* getBackground();
        void setBackground(Renderer::Renderer<NeoPixelBusType>* background);
        
        Renderer::Text<NeoPixelBusType>* getForeground();
        void setForeground(Renderer::Text<NeoPixelBusType>* foreground);
    };
}