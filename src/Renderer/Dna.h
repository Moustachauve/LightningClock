#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include <FastLED.h>

#include <Service/RendererManager.h>
#include "Renderer.h"

namespace Renderer {
    
    /**
     * By: ldirko (on Reddit)
     * https://pastebin.com/pCkkkzcs
     * https://old.reddit.com/user/ldirko
     *
     * Updated by: Preyy (on Reddit)
     */
    template<typename T_PIXEL_METHOD> class Dna : public Renderer<T_PIXEL_METHOD>
    {
    private:
        float mn =255.0/13.8;
        //awesome wu_pixel procedure by reddit u/sutaburosu
        void wu_pixel(uint32_t x, uint32_t y, CRGB * col) {
            auto leds = Service::RendererManager::Get().leds;
            // extract the fractional parts and derive their inverses
            uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
            // calculate the intensities for each affected pixel
            #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
            uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                            WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
            // multiply the intensities by the colour, and saturating-add them to the pixels
            for (uint8_t i = 0; i < 4; i++) {
                uint16_t xy = XY((x >> 8) + (i & 1), (y >> 8) + ((i >> 1) & 1));
                leds[xy].r = qadd8(leds[xy].r, col->r * wu[i] >> 8);
                leds[xy].g = qadd8(leds[xy].g, col->g * wu[i] >> 8);
                leds[xy].b = qadd8(leds[xy].b, col->b * wu[i] >> 8);
            }
        }
 
    protected:
        uint8_t speed = 10;
        uint16_t freq = 2500;

    public:
        Dna(T_PIXEL_METHOD* pMatrix) : Renderer<T_PIXEL_METHOD>(pMatrix) {};
        virtual ~Dna() {};

        virtual void Draw()
        {
            auto leds = Service::RendererManager::Get().leds;
            fadeToBlackBy(leds, LED_COUNT, 40);

            for (int i = 0; i < MATRIX_WIDTH; i++)
            {
                uint16_t ms = millis();
                uint32_t x = i * 256;
                uint32_t y1 = beatsin16(speed, 0, (MATRIX_HEIGHT - 1) * 256, 0, i * freq);
                uint32_t y2 = beatsin16(speed, 0, (MATRIX_HEIGHT - 1) * 256, 0, i * freq + 32768);

                CRGB col = CHSV(ms / 29 + i * 255 / (MATRIX_WIDTH - 1), 255, beatsin8(speed, 60, 255, 0, i * mn));
                CRGB col1 = CHSV(ms / 29 + i * 255 / (MATRIX_WIDTH - 1) + 128, 255, beatsin8(speed, 60, 255, 0, i * mn + 128));

                wu_pixel(x, y1, &col);
                wu_pixel(x, y2, &col1);
            }

            Cblur2d(leds, MATRIX_WIDTH, MATRIX_HEIGHT, 16);

            for (int i = 0; i < LED_COUNT; i++) {
                Renderer<T_PIXEL_METHOD>::matrix->SetPixelColor(i, RgbColor(leds[i].r, leds[i].g, leds[i].b));
            }
            };
    };
}