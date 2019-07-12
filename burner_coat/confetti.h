#include <FastLED.h>
#include "config.h"

void confetti() {
    int pos = random16(REAL_NUM_LEDS);

    if (pos > LEFT_MID && pos <= RIGHT_MID) {
        leds[pos] += LED_COLOR_HIGH;
    } else {
        leds[pos] += LED_COLOR_LOW;
    }
    delay(30);
}

void confettiToRainbow() {
    int i = random16(REAL_NUM_LEDS / 2);
    int j;
    CRGB color;
    for (j = 255; j > 0; j--) { // cycle of all colors on wheel
        color = wheel(((i * 256 / (STRIP_LENGTH)) + j) & 255);

        int l_idx = modulo(LEFT_TOP - i, REAL_NUM_LEDS);
        leds[l_idx] = color;

        int r_idx = modulo(RIGHT_TOP + i, REAL_NUM_LEDS);
        leds[r_idx] = color;
    }
    delay(5);
}

