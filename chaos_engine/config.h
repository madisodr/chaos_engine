#ifndef CONFIG_H
#define CONFIG_H

// default led pin for arduino boards
// LED Strip Output Pin
#define LED_PIN 3

// Maximum birghtness the LEDs can shine at.
#define MAX_BRIGHTNESS 127
#define MAX_VOLTS 300

#define PATTERN_LENGTH 15
#define NUM_LEDS 160
#define LEFT_TOP 62
#define RIGHT_TOP 63

#define DELAY_DRIFT 500
/*
    For 210 LEDs using the 60/3.3ft strips use
    #define NUM_LEDS 210
    #define LEFT_TOP 125
    #define RIGHT_TOP 126
*/

#define STRIP_LENGTH (NUM_LEDS / 2)

#define LED_COLOR_HIGH CHSV(0 + random8(36), 255, MAX_BRIGHTNESS)
#define LED_COLOR_LOW CHSV(100 + random8(36), 255, MAX_BRIGHTNESS)

#define RIGHT_MID (RIGHT_TOP + (STRIP_LENGTH/2))
#define LEFT_MID (LEFT_TOP - (STRIP_LENGTH/2))

#define LEFT_BOTTOM LEFT_TOP - STRIP_LENGTH
#define RIGHT_BOTTOM LEFT_BOTTOM - 1

#endif // CONFIG_H
