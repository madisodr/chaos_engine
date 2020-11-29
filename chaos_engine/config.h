#ifndef CONFIG_H
#define CONFIG_H

// default led pin for arduino boards
#define LED_PIN 3

// maximum birghtness the LEDs can shine at.
#define MAX_BRIGHTNESS 50

// maximum voltage FastLED will draw. Keeps power draw low.
#define MAX_VOLTS 100

// how long should each pattern runs for
// Seconds
#define PATTERN_LENGTH  20

// number of LEDs
#define NUM_LEDS 60

// used for making patterns symetrical. Since the 0th pixel on my LED strip wasn't
// in the center of my coat, I had to offset where the 0th' pixel of my LED strip is.
// offset from 0, this is the index of the bottom of the left side of my LED
#define LEFT_BOTTOM 60

// offset from 0, this is the index of the bottom of the right side of my LEDs
#define RIGHT_BOTTOM 61

// how fast should the delay increase or decrease during pattern switches
// a smaller number means the delay will drift faster.
#define DELAY_DRIFT 1000

/**************************
  Avoid modifying these unless you know what it will effect.
 **************************/

#define STRIP_LENGTH (NUM_LEDS / 2)
#define LEFT_TOP STRIP_LENGTH + LEFT_BOTTOM
#define RIGHT_TOP LEFT_TOP + 1

#define RIGHT_MID (RIGHT_TOP + (STRIP_LENGTH/2))
#define LEFT_MID (LEFT_TOP - (STRIP_LENGTH/2))

// #define LED_COLOR_HIGH CHSV(0 + random8(36), 255, MAX_BRIGHTNESS)
// #define LED_COLOR_LOW CHSV(100 + random8(36), 255, MAX_BRIGHTNESS)

#define BLEND_TIME_MULTIPLIER 2500L / 4

#endif // CONFIG_H
