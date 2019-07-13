#ifndef NOISE_H
#define NOISE_H

#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "palette.h"
#include "utils.h"

class Noise : public Pattern
{
    public:
        Noise(uint16_t _time, uint16_t _delay);
        ~Noise();

        void Generate(CRGB* arr);
        uint16_t XY(uint8_t x, uint8_t y);
        void MakeNoise();

        // The 16 bit version of̈ our coordinates. Should probably be private but
        static uint16_t x;
        static uint16_t y;
        static uint16_t z;
    private:
        const static uint8_t m_matrix_width  = 10;
        const static uint8_t m_matrix_height = 11;
        const static int m_max_dimension = ((m_matrix_width > m_matrix_height) ? m_matrix_width : m_matrix_height);

        // Speed of the pattern
        uint8_t m_speed;

        // Scale determines how far apart the pixels in our noise matrix are.
        const static uint8_t scale = 50;

        // This is the array that we keep our computed noise values in
        uint8_t m_noise[m_max_dimension][m_max_dimension];
};

uint16_t Noise::x;
uint16_t Noise::y;
uint16_t Noise::z;

Noise::Noise(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_speed = GetCurrentSpeed();
}

Noise::~Noise() {}

//
// Mark's xy coordinate mapping code.
//
uint16_t Noise::XY(uint8_t x, uint8_t y) {
    // Odd rows run backwards
    if (y & 0x01) {
        return (y * m_matrix_width) + (m_matrix_width - 1) - x;
    } else {
        // Even rows run forwards
        return (y * m_matrix_width) + x;
    }
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void Noise::MakeNoise() {
    // If we're runing at a low "speed", some 8-bit artifacts become visible
    // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
    // The amount of data smoothing we're doing depends on "speed".
    uint8_t dataSmoothing = 0;

    if (GetCurrentSpeed() < 50) {
        dataSmoothing = 200 - (GetCurrentSpeed() * 4);
    }

    for (int i = 0; i < m_max_dimension; i++) {
        int ioffset = scale * i;
        for (int j = 0; j < m_max_dimension; j++) {
            int joffset = scale * j * 4;

            uint8_t data = inoise8(x + ioffset, y + joffset, z);

            // The range of the inoise8 function is roughly 16-238.
            // These two operations expand those values out to roughly 0..255
            // You can comment them out if you want the raw noise data.
            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));

            if (dataSmoothing) {
                uint8_t oldData = m_noise[i][j];
                uint8_t newData = scale8(oldData, dataSmoothing) + scale8(data, 256 - dataSmoothing);
                data = newData;
            }

            m_noise[i][j] = data;
        }
    }

    z += GetCurrentSpeed();

    // apply slow drift to X and Y, just for visual variation.
    x += GetCurrentSpeed() / 8;
    y -= GetCurrentSpeed() / 16;
}

// heavily modified version of https://github.com/FastLED/FastLED/blob/master/examples/NoisePlusPalette/NoisePlusPalette.ino
void Noise::Generate(CRGB* arr) {
    // blend the current palette toward a new palette between 0 to 48 steps
    nblendPaletteTowardPalette(current_palette, target_palette, 24);

    // choose a new palette, speed, and scale
    updatePalette();

    // generate noise data
    MakeNoise();
    for (int i = 0; i < m_matrix_width; i++) {
        for (int j = 0; j < m_matrix_height; j++) {
            // We use the value at the (i,j) coordinate in the noise
            // array for our brightness, and the flipped value from (j,i)
            // for our pixel's index into the color palette.
            uint8_t index = m_noise[j][i];
            uint8_t bri =   m_noise[i][j];

            // brighten up, as the color palette itself often contains the
            // light/dark dynamic range desired
            if (bri > 127) {
                bri = 127;
            } else {
                bri = dim8_raw(bri * 2);
            }

            CRGB color = ColorFromPalette(getCurrentPalette(), index, bri);
            arr[modulo(XY(i, j), NUM_LEDS)] = color;
        }
    }
}

#endif
