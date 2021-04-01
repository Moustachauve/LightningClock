#include "RendererManager.h"

namespace Service 
{
    RendererManager::RendererManager()
    {
    }
    
    RendererManager::~RendererManager()
    {
    }

    RendererManager& RendererManager::Get()
    {
        static RendererManager instance;
        return instance;
    }

    Renderer::Renderer<NeoPixelBusType>* RendererManager::getBackground() 
    {
        return background;
    }

    void RendererManager::setBackground(Renderer::Renderer<NeoPixelBusType>* background) 
    {
        this->background = background;
    }
    
    Renderer::Text<NeoPixelBusType>* RendererManager::getForeground()
    {
        return foreground;
    }

    void RendererManager::setForeground(Renderer::Text<NeoPixelBusType>* foreground) 
    {
        this->foreground = foreground;
    }
}