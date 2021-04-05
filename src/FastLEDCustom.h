#pragma once

#include <FastLED.h>

uint16_t XY( uint8_t, uint8_t);// __attribute__ ((weak));

void CblurCols( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount);

void CblurRows(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount);

void Cblur2d( CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount);