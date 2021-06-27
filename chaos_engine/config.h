#ifndef CONFIG_H
#define CONFIG_H

// LED pins for strips
#define LED_PIN_A 3
#define LED_PIN_B 4
#define LED_PIN_C 5

#define REGULAR_STRIP 60
#define FOAM_PANEL 18

const int MAX_BRIGHTNESS            = 70; // maximum birghtness the LEDs can shine at.
const int MAX_VOLTS                 = 250; // max volts FastLED will draw. better power control
const int PATTERN_LENGTH            = 60; // how long should each pattern runs (SECONDS)
const int NUM_LEDS                  = FOAM_PANEL; // number of LEDs per strip
const int DELAY_DRIFT               = 1000; // pattern delay interpolation (MILLISECONDS)
const bool CONFIG_GLITCHING_ENABLED = true; // enables the glitching effects
const int NUM_LED_STRIPS            = 1;
/**************************
  Avoid modifying these unless you know what it will effect.
 **************************/

const int STRIP_LENGTH = int (NUM_LEDS / 2) + 1;

// #define LED_COLOR_HIGH CHSV(0 + random8(36), 255, MAX_BRIGHTNESS)
// #define LED_COLOR_LOW CHSV(100 + random8(36), 255, MAX_BRIGHTNESS)

#define BLEND_TIME_MULTIPLIER 2500L / 4

#endif // CONFIG_H
