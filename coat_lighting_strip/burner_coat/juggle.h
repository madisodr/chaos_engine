#include <FastLED.h>
#include "config.h"

const int dots = 2;
const CRGB c1 = P_ORG;
const CRGB c2 = P_TRL;

void juggle() {
  fadeToBlackBy(leds, NUM_LEDS, 40);
  for ( int i = 0; i < dots; i++) {
    if (i < dots / 2)
      leds[beatsin16( i + dots - 1, 0, REAL_NUM_LEDS)] = c1;
    else
      leds[beatsin16( i + dots - 1, 0, REAL_NUM_LEDS)] = c2;
  }
}
