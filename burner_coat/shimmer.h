#ifndef SHIMMER_H
#define SHIMMER_H

#include <FastLED.h>
#include "config.h"
#include "utils.h"

#define DELAY 20
class Shimmer : public Pattern
{
    public:
        Shimmer(uint16_t _time);
        ~Shimmer();

        void Run();
    private:

};

Shimmer::Shimmer(uint16_t _time) : Pattern(_time) {
    m_max_brightness = MAX_BRIGHTNESS;
}

Shimmer::~Shimmer() {

}

void Shimmer::Run() {
    fadeToBlackBy(leds, NUM_LEDS, 200);

    for (int i = 0; i < REAL_NUM_LEDS; i++) {
        if (i > LEFT_MID && i <= RIGHT_MID) {
            leds[i] = LED_COLOR_HIGH;
        } else {
            leds[i] = LED_COLOR_LOW;
        }
    }

    delay(DELAY);
}

#endif // SHIMMER_H
