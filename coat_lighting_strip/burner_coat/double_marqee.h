#include <FastLED.h>
#include "config.h"

const int marqee_length = 20;

void doubleMarqee() {
  CRGB color = P_ORG;
  int l_idx, r_idx;

  for (uint16_t q = 0; q < marqee_length; q++) {
    for (uint16_t i = 0; i < STRIP_LENGTH; i = i + marqee_length) {
      for (uint16_t z = 0; z < marqee_length; z++ ) {
        l_idx = modulo(LEFT_TOP - i - q + z, REAL_NUM_LEDS);
        leds[l_idx] = color;
        leds[l_idx].maximizeBrightness(10 + (z * 40));

        r_idx = modulo(RIGHT_TOP + i + q - z, REAL_NUM_LEDS);
        leds[r_idx] = color;
        leds[r_idx].maximizeBrightness(10 + (z * 40));
      }
    }

    FastLED.show();
    delay(70);
  }
}

void doubleMarqeeRainbow() {
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

    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.show();
    delay(10);
  }
}
