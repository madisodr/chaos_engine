#include <FastLED.h>
#include "config.h"

void shimmer() {
    fadeToBlackBy(leds, NUM_LEDS, 200);

    for (int i = 0; i < REAL_NUM_LEDS; i++) {
        if (i > LEFT_MID && i <= RIGHT_MID) {
            leds[i] = LED_COLOR_HIGH;
        } else {
            leds[i] = LED_COLOR_LOW;
        }
    }

    delay(20);
}
