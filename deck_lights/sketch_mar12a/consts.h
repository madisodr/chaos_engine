// default led pin for arduino boards
#define LED_PIN 3

// debug output Enabled
#define DEBUG (1)

enum Palette_Ids {
  PNK,
  PNJ,
  ORG,
  RNJ,
  Size
};

// pornj colors
#define P_PNK 0xFF1493
#define P_ORG 0xFF4500

extern const TProgmemPalette16 pink_palette_p PROGMEM =
{
  P_PNK, P_PNK, CRGB::Black, CRGB::Black,
  P_PNK, P_PNK, CRGB::Black, CRGB::Black,
  P_PNK, P_PNK, CRGB::Black, CRGB::Black,
  P_PNK, P_PNK, CRGB::Black, CRGB::Black
};

extern const TProgmemPalette16 org_palette_p PROGMEM =
{
  P_ORG, P_ORG, CRGB::Black, CRGB::Black,
  P_ORG, P_ORG, CRGB::Black, CRGB::Black,
  P_ORG, P_ORG, CRGB::Black, CRGB::Black,
  P_ORG, P_ORG, CRGB::Black, CRGB::Black
};

extern const TProgmemPalette16 pornj_palette_p PROGMEM =
{
  P_PNK, P_PNK, CRGB::Black, CRGB::Black,
  P_ORG, P_ORG, CRGB::Black, CRGB::Black,
  P_PNK, P_PNK, CRGB::Black, CRGB::Black,
  P_ORG, P_ORG, CRGB::Black, CRGB::Black
};

extern const TProgmemPalette16 r_pornj_palette_p PROGMEM =
{
  P_PNK, P_ORG, CRGB::Black, CRGB::Black,
  P_ORG, P_PNK, CRGB::Black, CRGB::Black,
  P_PNK, P_ORG, CRGB::Black, CRGB::Black,
  P_ORG, P_PNK, CRGB::Black, CRGB::Black
};
