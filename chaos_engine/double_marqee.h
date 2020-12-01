#ifndef DOUBLE_MARQEE_H
#define DOUBLE_MARQEE_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class DoubleMarqee : public Pattern
{
    public:
        DoubleMarqee(uint16_t _time, uint16_t _delay);
        ~DoubleMarqee();

        void Generate(CRGB* arr);
        void Reset();
    private:
        uint8_t m_hue;
        uint8_t m_num_leds;
};

DoubleMarqee::DoubleMarqee(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_hue = 0;
    m_num_leds = 0;
}

inline void DoubleMarqee::Reset()
{
    m_num_leds = 0;
    m_hue = 0;
}

DoubleMarqee::~DoubleMarqee() {}


void DoubleMarqee::Generate(CRGB* arr)
{
    m_hue++;
    CRGB color;
    for (uint8_t i = 0; i < STRIP_LENGTH; i++) {
        
        if (i > m_num_leds) {
            continue;
        }

        if (m_reverse) {
            color = wheel(((i * 256 / (STRIP_LENGTH)) - m_hue) & 255);
        } else {
            color = wheel(((i * 256 / (STRIP_LENGTH)) + m_hue) & 255);
        }

        uint8_t l_idx = modulo(STRIP_LENGTH - i, NUM_LEDS);
        arr[l_idx] = color;

        uint8_t r_idx = modulo((STRIP_LENGTH + i, NUM_LEDS);
        arr[r_idx] = color;
    }

    if (m_num_leds >= NUM_LEDS) {
        m_num_leds = NUM_LEDS;
    } else {
        EVERY_N_MILLISECONDS(100) {
            m_num_leds += 1;
        }
    }
}

#endif // DOUBLE_MARQEE_H
