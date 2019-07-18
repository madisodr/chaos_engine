#ifndef FIRE_H
#define FIRE_H

#include <FastLED.h>
#include "config.h"

DEFINE_GRADIENT_PALETTE(bluemap) {
    50,    0,   0,   0,     //black
    80,    255, 255, 244,   // teal
    100,   255, 0,   100,  // pink
    255,   255, 0,   100  // pink
};

class Fire : public Pattern
{
    public:
        Fire(uint16_t _time, uint16_t _delay);
        ~Fire();

        void Generate(CRGB* arr);
        void Reset();
    private:
        uint32_t m_xscale; // distance
        uint32_t m_yscale; // speed
        uint8_t m_index;
        CRGBPalette16 m_palette;
};

Fire::Fire(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_palette = bluemap;
    m_xscale = 50;
    m_yscale = 5;
    m_index = 0;
}

Fire::~Fire() {}

void Fire::Generate(CRGB* arr)
{
    for (int i = 0; i < STRIP_LENGTH; i++) {

        uint8_t l_idx = modulo(LEFT_TOP - i, NUM_LEDS);
        uint8_t r_idx = modulo(RIGHT_TOP + i, NUM_LEDS);

        m_index = inoise8(i * m_xscale, millis() * m_yscale * STRIP_LENGTH / 255);

        arr[l_idx] = ColorFromPalette(m_palette, min(i * (m_index) >> 6, 255), i * 255 / STRIP_LENGTH, LINEARBLEND);
        arr[r_idx] = ColorFromPalette(m_palette, min(i * (m_index) >> 6, 255), i * 255 / STRIP_LENGTH, LINEARBLEND);
    }
}

void Fire::Reset()
{
    m_index = 0;
}


#endif // PATTERN_NAME_H
