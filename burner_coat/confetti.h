#ifndef CONFETTI_H
#define CONFETTI_H

#include <FastLED.h>
#include "config.h"

class Confetti : public Pattern
{
    public:
        Confetti(uint16_t _time, uint16_t _delay);
        ~Confetti();
        void Generate(CRGB* arr);
};

Confetti::Confetti(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) {}
Confetti::~Confetti() {}

void Confetti::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 30);

    uint8_t pos = random8(NUM_LEDS);

    if (pos > LEFT_MID && pos <= RIGHT_MID) {
        arr[pos] += LED_COLOR_HIGH;
    } else {
        arr[pos] += LED_COLOR_LOW;
    }
}

#endif // CONFETTI_H
