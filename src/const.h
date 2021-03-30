#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include <NeoPixelBrightnessBus.h>

#define LED_PIN 2

#define MATRIX_WIDTH 27
#define MATRIX_HEIGHT 8
#define LED_COUNT (MATRIX_WIDTH * MATRIX_HEIGHT)

typedef NeoPixelBrightnessBusGfx<NeoGrbwFeature, Neo800KbpsMethod>  NeoPixelBusType;
