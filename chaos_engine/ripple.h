#ifndef RIPPLE_H
#define RIPPLE_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class Ripple : public Pattern
{
    public:
        Ripple(uint16_t _time, uint16_t _delay);
        ~Ripple();

        void Generate(CRGB* arr);
        void Reset();
    private:
        uint8_t m_hue;
        int8_t m_step;
        uint8_t m_max_steps;
};

Ripple::Ripple(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_step = -1;
    m_max_steps = 15;
}

Ripple::~Ripple() {}
inline void Ripple::Reset() {}

void Ripple::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 64);
    int8_t _center;
    CRGB _color;
    
    if (m_step == -1) {
        _center = random(NUM_LEDS);
        _color = wheel(Pattern::GetGlobalHue());
        m_step = 0;
    } else if (m_step == 0) {
        arr[_center] = _color;
        m_step++;
    } else if (m_step < m_max_steps) {
        arr[(_center + m_step + NUM_LEDS) % NUM_LEDS] = _color;
        arr[(_center - m_step + NUM_LEDS) % NUM_LEDS] = _color;

        arr[(_center + (NUM_LEDS / random8(4)) + m_step + NUM_LEDS) % NUM_LEDS] = _color;
        arr[(_center + (NUM_LEDS / random8(4)) - m_step + NUM_LEDS) % NUM_LEDS] = _color;
        
        m_step++;
    } else {
        m_step = -1;
    }

    blur1d(arr, NUM_LEDS, 150);
}

#endif // RIPPLE_H
