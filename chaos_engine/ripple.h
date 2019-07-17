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
        uint8_t m_color;
        uint8_t m_center;
        uint8_t m_step;
        uint8_t m_max_steps = 20;
        float m_fade_rate = .9;
        uint8_t m_diff;

        uint8_t Wrap(uint8_t _step);
};

Ripple::Ripple(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_center = 0;
    m_step = -1;
}

Ripple::~Ripple() {}
inline void Ripple::Reset() {}

inline uint8_t Ripple::Wrap(uint8_t _step)
{
    if (_step < 0) {
        return NUM_LEDS + _step;
    } else if (_step > NUM_LEDS - 1) {
        return _step - NUM_LEDS;
    } else {
        return _step;
    }
}

void Ripple::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 30);

    if (m_step == -1) {
        m_center = random(NUM_LEDS);

        if (m_center > LEFT_MID && m_center <= RIGHT_MID) {
            m_color = 0 + random8(20);
        } else {
            m_color = 130 + random8(10);
        }
        m_step = 0;
    }

    if (m_step == 0) {
        arr[m_center] = CHSV(m_color, 255, MAX_BRIGHTNESS);
        m_step ++;
    } else {
        if (m_step < m_max_steps) {
            arr[Wrap(m_center + m_step)] = CHSV(m_color, 255, pow(m_fade_rate, m_step) * MAX_BRIGHTNESS);
            arr[Wrap(m_center - m_step)] = CHSV(m_color, 255, pow(m_fade_rate, m_step) * MAX_BRIGHTNESS);
            if (m_step > 3) {
                arr[Wrap(m_center + m_step - 3)] = CHSV(m_color, 255, pow(m_fade_rate, m_step - 2) * MAX_BRIGHTNESS);
                arr[Wrap(m_center - m_step + 3)] = CHSV(m_color, 255, pow(m_fade_rate, m_step - 2) * MAX_BRIGHTNESS);
            }
            m_step++;
        } else {
            m_step = -1;
        }
    }

    blur1d(arr, NUM_LEDS, .1);
}

#endif // RIPPLE_H
