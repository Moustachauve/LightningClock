#include "FastLEDCustom.h"

uint16_t XY( uint8_t, uint8_t);// __attribute__ ((weak));

// blurCols: perform a blur1d on every column of a rectangular matrix
void CblurCols( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
    for( uint8_t col = 0; col < width; ++col) {
        CRGB* col_base = leds + col * height;
        blur1d( col_base, height, blur_amount);
    }
}

// CblurRows: perform a blur1d on each row of a rectangular matrix
void CblurRows(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
    // blur rows
    uint8_t keep = 255 - blur_amount;
    uint8_t seep = blur_amount >> 1;
    for( uint8_t row = 0; row < height; ++row) {
        CRGB carryover = CRGB::Black;
        for( uint8_t i = 0; i < width; ++i) {
            CRGB cur = leds[XY(i, row)];
            CRGB part = cur;
            part.nscale8( seep);
            cur.nscale8( keep);
            cur += carryover;
            if( i) leds[XY(i-1, row)] += part;
            leds[XY(i, row)] = cur;
            carryover = part;
        }
    }
}

void Cblur2d( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
    CblurCols(leds, width, height, blur_amount);
    CblurRows(leds, width, height, blur_amount);
}