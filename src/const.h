#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include <NeoPixelBrightnessBus.h>

#define LED_PIN 2

#define MATRIX_WIDTH 27
#define MATRIX_HEIGHT 8
#define LED_COUNT (MATRIX_WIDTH * MATRIX_HEIGHT)

typedef NeoPixelBrightnessBusGfx<NeoGrbwFeature, Neo800KbpsMethod> NeoPixelBusType;

//typedef NeoTopology<RowMajorAlternating90Layout> NeoPixelTopology; // Upside down
typedef NeoTopology<RowMajorAlternating270Layout> NeoPixelTopology;


#define TARGET_FPS 60
#define TARGET_FRAMETIME (1000/TARGET_FPS)

#define DEFAULT_BACKGROUND_COLOR 0x00FFFF
#define DEFAULT_FOREGROUND_COLOR 0x000000FF