#include <FastLED.h>
#include "config.h"

int color;
int center = 0;
int step = -1;
const int maxSteps = 15;
const float fadeRate = .9;
int diff;

inline int wrap(int step) {
  if (step < 0) return REAL_NUM_LEDS + step;
  if (step > REAL_NUM_LEDS - 1) return step - REAL_NUM_LEDS;
  return step;
}

void ripple() {
  fadeToBlackBy(leds, REAL_NUM_LEDS, 5);

  if (step == -1) {
    center = random(REAL_NUM_LEDS);

    if (center > LEFT_MID && center <= RIGHT_MID) {
      color = 20 + random8(36);
    } else {
      color = 120 + random8(36);;
    }
    step = 0;
  }
  if (step == 0) {
    leds[center] = CHSV(color, 255, MAX_BRIGHTNESS);
    step ++;
  } else {
    if (step < maxSteps) {
      leds[wrap(center + step)] = CHSV(color, 255, pow(fadeRate, step) * MAX_BRIGHTNESS);
      leds[wrap(center - step)] = CHSV(color, 255, pow(fadeRate, step) * MAX_BRIGHTNESS);
      if (step > 3) {
        leds[wrap(center + step - 3)] = CHSV(color, 255, pow(fadeRate, step - 2) * MAX_BRIGHTNESS);
        leds[wrap(center - step + 3)] = CHSV(color, 255, pow(fadeRate, step - 2) * MAX_BRIGHTNESS);
      }
      step ++;
    } else {
      step = -1;
    }
  }

  delay(50);
}
