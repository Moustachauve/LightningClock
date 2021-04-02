#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include "Renderer.h"
#include<Fonts/m3x6.h>
#include<Fonts/m5x7.h>

namespace Renderer {
    
    enum TextAlignment
    {
        TextAlignLeft = 0,
        TextAlignCenter = 1,
        TextAlignRight = 2,
        TextAlignMarqueeLeft = 3,
        TextAlignMarqueeRight = 4,
        TextAlignMarqueeAlternate = 5,
        TextAlignMarqueeBounce = 6
    };

    template<typename T_PIXEL_METHOD> class Text : public Renderer<T_PIXEL_METHOD>
    {
    private:
        const char* text;
        int16_t x = 0;
        int16_t offsetX = 0;
        unsigned long prevMillis = 0;
        uint8_t speed = 85;
        int8_t direction = 1;
        RgbwColor color = HtmlColor(DEFAULT_FOREGROUND_COLOR);
        GFXfont* font = NULL;
        TextAlignment alignment = TextAlignLeft;

        bool shouldMove()
        {
            unsigned long curMillis = millis();  
            if (curMillis - prevMillis >= speed) {
                prevMillis = curMillis;
                return true;
            }

            return false;
        }

    public:
        Text(T_PIXEL_METHOD* pMatrix) : Renderer<T_PIXEL_METHOD>(pMatrix) {};
        virtual ~Text() {};
        virtual void Draw()
        {    
            Renderer<T_PIXEL_METHOD>::matrix->setFont(font);


            int16_t  x1, y1;
            uint16_t w, h;
            Renderer<T_PIXEL_METHOD>::matrix->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
            switch (alignment)
            {
            case TextAlignCenter:
                x = (MATRIX_WIDTH - w) / 2;
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, !font ? 0 : 6);
                break;
                
            case TextAlignRight:
                x = MATRIX_WIDTH - w + offsetX;
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, !font ? 0 : 6);
                break;
                
            case TextAlignMarqueeLeft:
                if (shouldMove() && --x <= -(w + offsetX)) {
                    x = MATRIX_WIDTH;
                }
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, !font ? 0 : 6);
                break;

            case TextAlignMarqueeRight:
                if (shouldMove() && ++x >= MATRIX_WIDTH) {
                    x = -w;
                }
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, !font ? 0 : 6);
                break;

            case TextAlignMarqueeAlternate:
                if (shouldMove()) {
                    x += direction;
                    if (x <= -(w + offsetX)) {
                        direction = 1;
                    } else if (x >= MATRIX_WIDTH + offsetX) {
                        direction = -1;
                    }
                }
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, !font ? 0 : 6);
                break;

            case TextAlignMarqueeBounce:
                if (shouldMove()) {
                    if (w + offsetX < MATRIX_WIDTH) {
                        x += direction;
                        if (x <= offsetX) {
                            direction = 1;
                        } else if (x + w >= MATRIX_WIDTH + offsetX) {
                            direction = -1;
                        }
                    } else if (w + offsetX > MATRIX_WIDTH) {
                        x += direction;
                        if (x + w <= MATRIX_WIDTH + offsetX) {
                            direction = 1;
                        } else if (x >= offsetX) {
                            direction = -1;
                        }
                    }
                }
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(x, !font ? 0 : 6);
                break;
            
            case TextAlignLeft:
            default:
                Renderer<T_PIXEL_METHOD>::matrix->setCursor(0 + offsetX, !font ? 0 : 6);
                break;
            }            

            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor(color);
            Renderer<T_PIXEL_METHOD>::matrix->print(text);
            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor();
        };
        void setText(const char* pText) 
        {
            text = pText;
        };
        void setColor(RgbwColor pColor)
        {
            color = pColor;
        }
        void setAlignment(TextAlignment pAlignment)
        {
            alignment = pAlignment;
        }
        void setSpeed(uint8_t pSpeed)
        {
            speed = pSpeed;
        }
        void setFont(const GFXfont* pFont)
        {
            if (!pFont) {
                font = NULL;
                return;
            }
            font = (GFXfont *)pFont;
        }
        void setOffsetX(int16_t pOffset)
        {
            offsetX = pOffset;
        }
    };
}