#include <FastLED.h>
#include "consts.h"

const uint8_t kMatrixWidth  = 16;
const uint8_t kMatrixHeight = 16;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

// The leds
CRGB leds[NUM_LEDS];

// The 16 bit version of̈ our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;

// Speed of the patterns
uint16_t speed = 1;

// Scale determines how far apart the pixels in our noise matrix are.
uint16_t scale = 50;

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

CRGBPalette16 currentPalette(pink_palette_p);
CRGBPalette16 targetPalette;

/*
   setup
*/
void setup() {
  LEDS.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);

  // Initialize our coordinates to some random values
  x = random16();
  y = random16();
  z = random16();

#if(DEBUG)
  Serial.begin(9600);
#endif

}

/*
   main loop
*/
void loop()
{
  // blend the current palette toward a new palette between 0 to 48 steps
  nblendPaletteTowardPalette(currentPalette, targetPalette, 24);

  // choose a new palette, speed, and scale
  update_palette();

  // generate noise data
  generate_noise();

  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette();
  //juggle();
  
  // A more power efficient way to show the LEDs
  set_max_power_in_volts_and_milliamps(5, 500);
  show_at_max_brightness_for_power();
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  const int dots = 6;
  
  fadeToBlackBy(leds, 120, 40);
  for( int i = 0; i < dots; i++) {
    if (i % 2 == 0) 
      leds[beatsin16( i+dots-1, 0, 120-1 )] = P_PNK;
    else
      leds[beatsin16( i+dots-1, 0, 120-1 )] = P_ORG;
  }
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void generate_noise() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;

  if (speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }

  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j * 4;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if (dataSmoothing) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 4;
}

void mapNoiseToLEDsUsingPalette() {
  static uint8_t ihue = 0;

  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.
      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // add a slowly-changing base value
      // index += ihue;

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if (bri > 127) {
        bri = 127;
      } else {
        bri = dim8_raw(bri * 2);
      }

      CRGB color = ColorFromPalette(currentPalette, index, bri);
      leds[XY(i, j)] = color;
    }
  }

  ihue += 1;
}

void update_palette() {
  uint8_t hold_palette_x_times_as_long = 1;
  uint8_t speed_step = 1;
  static uint8_t palette_idx = Palette_Ids::PNK; // index for the palettes array

  uint8_t secondHand = ((millis() / 1000) / hold_palette_x_times_as_long) % 60;
  static uint8_t lastSecond = 99;

  CRGBPalette16 palettes[Palette_Ids::Size] = {pink_palette_p, pornj_palette_p, org_palette_p, r_pornj_palette_p};

  if (lastSecond != secondHand) {
    lastSecond = secondHand;
    
    if (secondHand % 5 == 0) {
      if (secondHand < 30) {
        speed += speed_step;
      } else {
        speed -= speed_step;
      }
      scale = 30;
      palette_idx++;
    }

    // wrap the palette index around to 0
    palette_idx = palette_idx % Palette_Ids::Size;

    targetPalette = palettes[palette_idx];

#if(DEBUG)
    Serial.print("Idx: ");
    Serial.println(palette_idx);
    String p;
    if (palette_idx == 0) p = " PINK";
    else if (palette_idx == 1) p = " PORNJ";
    else if (palette_idx == 2) p = " ORANGE";
    else if (palette_idx == 3) p = " REV PORNJ";

    Serial.println(p);
    Serial.print("Speed: "); Serial.println(speed);
    Serial.print("Scale: "); Serial.println(scale);
    Serial.print("Second: "); Serial.println(secondHand);
    Serial.println("---");
#endif
  }
}

//
// Mark's xy coordinate mapping code.  See the XYMatrix for more information on it.
//
uint16_t XY(uint8_t x, uint8_t y)
{
  uint16_t i;

  if (y & 0x01) {
    // Odd rows run backwards
    uint8_t reverseX = (kMatrixWidth - 1) - x;
    i = (y * kMatrixWidth) + reverseX;
  } else {
    // Even rows run forwards
    i = (y * kMatrixWidth) + x;
  }

  return i;
}

