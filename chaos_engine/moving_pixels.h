#ifndef MOVING_PIXELS_H
#define MOVING_PIXELS_H

#include <FastLED.h>
#include "config.h"

class MovingPixels : public Pattern
{
    public:
        MovingPixels(uint16_t _time, uint16_t _delay);
        ~MovingPixels();

        void Generate(CRGB* arr);
        void Reset();
    private:
        uint8_t m_pos;
        uint8_t m_pixel_count;
};

MovingPixels::MovingPixels(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_pixel_count = random(4,8);
    m_pos = 0;
}

inline void MovingPixels::Reset()
{
    m_pixel_count = random(4,8);
    ToggleReverse();
}

MovingPixels::~MovingPixels() {}

void MovingPixels::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 128);

    uint8_t pixel_distance = NUM_LEDS / m_pixel_count;

    for (int i = 0; i < m_pixel_count; i++) {
        int pos = modulo(m_pos + (pixel_distance * i), NUM_LEDS);
        
        arr[pos] = wheel(Pattern::GetGlobalHue());
    }

    if (m_reverse) {
        m_pos = modulo(m_pos - 1, NUM_LEDS);
    } else {
        m_pos = modulo(m_pos + 1, NUM_LEDS);
    }
}

#endif // MOVING_PIXELS_H
