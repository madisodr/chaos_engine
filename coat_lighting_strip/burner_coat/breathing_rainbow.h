#include <FastLED.h>
#include "config.h"
#include "utils.h"

// speed of the breathing rainbow sin milliseconds
const int breath_speed = 5000.0 * PI;

// current color to pick from the rainbow
static uint8_t breathing_rainbow_hue = 0;

void breathingRainbow() {  
  float breath = (sin(millis() / breath_speed)) * 108.0;
  int starting_pos = 0;
  breath = map(breath, 0, MAX_BRIGHTNESS, 5, 40);
  FastLED.setBrightness(breath);

  // increase the hue by 1 each time
  breathing_rainbow_hue++;
  byte hue_delta = 255 / REAL_NUM_LEDS;

  fill_rainbow(leds, REAL_NUM_LEDS, breathing_rainbow_hue, hue_delta);
  delay(20);
}
