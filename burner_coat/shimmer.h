#ifndef SHIMMER_H1
#define SHIMMER_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class Shimmer : public Pattern
{
    public:
        Shimmer(uint16_t _time);
        ~Shimmer();

        void Generate(CRGB* arr);
    private:
};

Shimmer::Shimmer(uint16_t _time) : Pattern(_time)
{
    m_max_brightness = MAX_BRIGHTNESS;
}

Shimmer::~Shimmer() {}

void Shimmer::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 40);
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i > LEFT_MID && i <= RIGHT_MID) {
            arr[i] = LED_COLOR_HIGH;
        } else {
            arr[i] = LED_COLOR_LOW;
        }
    }

    delay(20);
}

#endif // SHIMMER_H
