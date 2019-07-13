#ifndef CONFIG_H
#define CONFIG_H

// default led pin for arduino boards
// LED Strip Output Pin
#define LED_PIN 3

const uint8_t kMatrixWidth  = 10;
const uint8_t kMatrixHeight = 11;

#define NUM_LEDS 106 // (kMatrixWidth * kMatrixHeight)
//#define REAL_NUM_LEDS 106
#define STRIP_LENGTH (NUM_LEDS / 2)
#define MAX_DIMENSION ((kMatrixWidth > kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

#define MAX_BRIGHTNESS 150
// The leds
CRGB leds[NUM_LEDS];

#define MAX_VOLTS 500

#define LED_COLOR_HIGH CHSV(0 + random8(36), 255, 255)
#define LED_COLOR_LOW CHSV(100 + random8(36), 255, 255)

const int LEFT_TOP = 62;
const int RIGHT_TOP = 63;

const int RIGHT_MID = RIGHT_TOP + (STRIP_LENGTH/2) - 5;
const int LEFT_MID = LEFT_TOP - (STRIP_LENGTH/2) + 5;

// Define a better Orange
#define P_ORG 0xFF4500
#define P_TRL 0x00F2FF

// ID's for each palette in our program
enum Palette_Ids {
    MIX,
    ORG
};
/*
extern const TProgmemPalette16 orangePalette PROGMEM =
{
    P_ORG, CRGB::Black, CRGB::Black, CRGB::Black,
    P_ORG, CRGB::Black, CRGB::Black, CRGB::Black,
    CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
    CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

extern const TProgmemPalette16 mixedPalette PROGMEM =
{
    P_ORG, CRGB::Black, CRGB::Black, CRGB::Black,
    P_TRL, CRGB::Black, CRGB::Black, CRGB::Black,
    P_TRL, CRGB::Black, CRGB::Black, CRGB::Black,
    P_ORG, CRGB::Black, CRGB::Black, CRGB::Black
};
*/
#endif // CONFIG_H
