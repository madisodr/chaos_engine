 #ifndef CONFIG_H
#define CONFIG_H

// default led pin for arduino boards
// LED Strip Output Pin
#define LED_PIN 3

#define NUM_LEDS 106
#define STRIP_LENGTH (NUM_LEDS / 2)

#define MAX_BRIGHTNESS 40
// The leds
CRGB leds[NUM_LEDS];

#define MAX_VOLTS 100

#define LED_COLOR_HIGH CHSV(0 + random8(36), 255, MAX_BRIGHTNESS)
#define LED_COLOR_LOW CHSV(100 + random8(36), 255, MAX_BRIGHTNESS)

const uint8_t LEFT_TOP = 62;
const uint8_t RIGHT_TOP = 63;

const uint8_t RIGHT_MID = RIGHT_TOP + (STRIP_LENGTH/2);
const uint8_t LEFT_MID = LEFT_TOP - (STRIP_LENGTH/2);

// Define a better Orange
#define P_ORG 0xFF4500
#define P_TRL 0x00F2FF

#endif // CONFIG_H
