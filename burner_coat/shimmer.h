#ifndef SHIMMER_H
#define SHIMMER_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class Shimmer : public Pattern
{
    public:
        Shimmer(uint16_t _time, uint16_t _delay);
        ~Shimmer();

        void Generate(CRGB* arr);
    private:
};

Shimmer::Shimmer(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) {}

Shimmer::~Shimmer() {}

void Shimmer::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 80);
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i > LEFT_MID && i <= RIGHT_MID) {
            arr[i] = LED_COLOR_HIGH;
        } else {
            arr[i] = LED_COLOR_LOW;
        }
    }
}

#endif // SHIMMER_H
