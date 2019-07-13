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

    private:
        const byte m_delta = 255 / NUM_LEDS;
        uint8_t m_hue;

};

DoubleMarqee::DoubleMarqee(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_hue = 0;
}

DoubleMarqee::~DoubleMarqee() {}


void DoubleMarqee::Generate(CRGB* arr)
{
    m_hue++;
    for (uint16_t i = 0; i < STRIP_LENGTH; i++) {

        if (i > m_num_leds)
            continue;

        CRGB color = wheel(((i * 256 / (STRIP_LENGTH)) + m_hue) & 255);
        int l_idx = modulo(LEFT_TOP - i, NUM_LEDS);
        arr[l_idx] = color;

        int r_idx = modulo(RIGHT_TOP + i, NUM_LEDS);
        arr[r_idx] = color;
    }

    EVERY_N_MILLISECONDS(100) {
        m_num_leds += 1;
    }

    if (m_num_leds >= NUM_LEDS) {
        m_num_leds = NUM_LEDS;
    }
}


#endif // DOUBLE_MARQEE_H
