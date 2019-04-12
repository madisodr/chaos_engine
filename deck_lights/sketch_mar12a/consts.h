/*** LED ***/
// default led pin for arduino boards
#define LED_PIN 7

// default brightness for leds
#define BRIGHTNESS 100

// led type for specs
#define LED_TYPE WS2811

// color order
#define COLOR_ORDER GRB

enum Palette_Color_IDs {
  PNK = 0,
  PNJ = 1,
  ORG = 2,
  RNJ = 3 
};

// pornj colors
#define P_PNK 0xFF1493
#define P_ORG 0xFF4500
#define P_PRP 0x800080

extern CRGBPalette16 pink_palette;
extern CRGBPalette16 org_palette;
extern CRGBPalette16 pornj_palette;
extern CRGBPalette16 r_pornj_palette;

extern const TProgmemPalette16 pink_palette_p PROGMEM;
const TProgmemPalette16 pink_palette_p PROGMEM =
{
    P_PNK, P_PNK, CRGB::Black, CRGB::Black,
    P_PNK, P_PNK, CRGB::Black, CRGB::Black,
    P_PNK, P_PNK, CRGB::Black, CRGB::Black,
    P_PNK, P_PNK, CRGB::Black, CRGB::Black
};

extern const TProgmemPalette16 org_palette_p PROGMEM;
const TProgmemPalette16 org_palette_p PROGMEM =
{
    P_ORG, P_ORG, CRGB::Black, CRGB::Black,
    P_ORG, P_ORG, CRGB::Black, CRGB::Black,
    P_ORG, P_ORG, CRGB::Black, CRGB::Black,
    P_ORG, P_ORG, CRGB::Black, CRGB::Black
};

extern const TProgmemPalette16 pornj_palette_p PROGMEM;
const TProgmemPalette16 pornj_palette_p PROGMEM =
{
    P_PNK, P_PNK, CRGB::Black, CRGB::Black,
    P_ORG, P_ORG, CRGB::Black, CRGB::Black,
    P_PNK, P_PNK, CRGB::Black, CRGB::Black,
    P_ORG, P_ORG, CRGB::Black, CRGB::Black
};

extern const TProgmemPalette16 r_pornj_palette_p PROGMEM;
const TProgmemPalette16 r_pornj_palette_p PROGMEM =
{
    P_PNK, P_ORG, CRGB::Black, CRGB::Black,
    P_ORG, P_PNK, CRGB::Black, CRGB::Black,
    P_PNK, P_ORG, CRGB::Black, CRGB::Black,
    P_ORG, P_PNK, CRGB::Black, CRGB::Black
};
