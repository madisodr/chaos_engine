#ifndef NOISE_H
#define NOISE_H

#include <FastLED.h>
#include "config.h"

// The 16 bit version of̈ our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;

// Speed of the pattern
static uint8_t speed = 1;

// Scale determines how far apart the pixels in our noise matrix are.
const uint8_t scale = 50;

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

inline CRGBPalette16 getCurrentPalette();
//
// Mark's xy coordinate mapping code.
//
uint16_t XY(uint8_t x, uint8_t y)
{
    // Odd rows run backwards
    if (y & 0x01) {
        return (y * kMatrixWidth) + (kMatrixWidth - 1) - x;
    } else {
        // Even rows run forwards
        return (y * kMatrixWidth) + x;
    }
}


// Fill the x/y array of 8-bit noise values using the inoise8 function.
void generateNoise()
{
    // If we're runing at a low "speed", some 8-bit artifacts become visible
    // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
    // The amount of data smoothing we're doing depends on "speed".
    uint8_t dataSmoothing = 0;

    if (speed < 50) {
        dataSmoothing = 200 - (speed * 4);
    }

    for (int i = 0; i < MAX_DIMENSION; i++) {
        int ioffset = scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++) {
            int joffset = scale * j * 4;

            uint8_t data = inoise8(x + ioffset, y + joffset, z);

            // The range of the inoise8 function is roughly 16-238.
            // These two operations expand those values out to roughly 0..255
            // You can comment them out if you want the raw noise data.
            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            if (dataSmoothing) {
                uint8_t oldData = noise[i][j];
                uint8_t newData = scale8(oldData, dataSmoothing) + scale8(data, 256 - dataSmoothing);
                data = newData;
            }

            noise[i][j] = data;
        }
    }

    z += speed;

    // apply slow drift to X and Y, just for visual variation.
    x += speed / 8;
    y -= speed / 16;
}

// heavily modified version of https://github.com/FastLED/FastLED/blob/master/examples/NoisePlusPalette/NoisePlusPalette.ino
void noisy()
{

    for (int i = 0; i < kMatrixWidth; i++) {
        for (int j = 0; j < kMatrixHeight; j++) {
            // We use the value at the (i,j) coordinate in the noise
            // array for our brightness, and the flipped value from (j,i)
            // for our pixel's index into the color palette.
            uint8_t index = noise[j][i];
            uint8_t bri =   noise[i][j];

            // brighten up, as the color palette itself often contains the
            // light/dark dynamic range desired
            if (bri > 127) {
                bri = 127;
            } else {
                bri = dim8_raw(bri * 2);
            }

            CRGB color = ColorFromPalette(getCurrentPalette(), index, bri);
            leds[XY(i, j)] = color;
        }
    }
    delay(30);
}

#endif
