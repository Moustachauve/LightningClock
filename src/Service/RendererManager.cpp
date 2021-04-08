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

    void RendererManager::Draw()
    {
        uint32_t now = millis();
        if (now > nextTime) {
            yield();
            background->Draw();
            yield();
            foreground->Draw();

            nextTime = now + TARGET_FRAMETIME;
        }
    }

    uint16_t RendererManager::Map(uint16_t x, uint16_t y) {
        return topo.Map(x, y);
    }
    
    uint32_t RendererManager::crgb_to_col(CRGB fastled)
    {
        return (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
    }

    void RendererManager::OnWsEvent(AsyncWebSocket * wsServer, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
    {
        background->OnWsEvent(wsServer, client, type, arg, data, len);
        foreground->OnWsEvent(wsServer, client, type, arg, data, len);
    }
}