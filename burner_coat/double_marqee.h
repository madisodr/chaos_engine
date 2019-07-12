#ifndef DOUBLE_MARQEE_H
#define DOUBLE_MARQEE_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class DoubleMarqee : public Pattern
{
    public:
        DoubleMarqee(uint16_t _time);
        ~DoubleMarqee();

        void Run();
    private:
        const int marqee_length = 12;
};

DoubleMarqee::DoubleMarqee(uint16_t _time): Pattern(_time) {
}

DoubleMarqee::~DoubleMarqee() {

}

void DoubleMarqee::Run() {
    CRGB color;
    int l_idx, r_idx;

    for (uint16_t j = 255; j > 0; j--) { // cycle of all colors on wheel
        for (uint16_t i = 0; i < STRIP_LENGTH; i++) {
            color = wheel(((i * 256 / (STRIP_LENGTH)) + j) & 255);
            l_idx = modulo(LEFT_TOP - i, REAL_NUM_LEDS);
            leds[l_idx] = color;

            r_idx = modulo(RIGHT_TOP + i, REAL_NUM_LEDS);
            leds[r_idx] = color;
        }

        delay(10);
    }

}

#endif // DOUBLE_MARQEE_H
