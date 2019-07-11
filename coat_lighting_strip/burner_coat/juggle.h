#include <FastLED.h>
#include "config.h"

void juggle()
{
    const int dots = 2;
    fadeToBlackBy(leds, NUM_LEDS, 40);
    for ( int i = 0; i < dots; i++) {
        if (i < dots / 2)
            leds[beatsin16( i + dots - 1, 0, REAL_NUM_LEDS)] = P_ORG;
        else
            leds[beatsin16( i + dots - 1, 0, REAL_NUM_LEDS)] = P_TRL;
    }
}
