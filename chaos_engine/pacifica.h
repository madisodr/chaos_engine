#include <FastLED.h>
#include "config.h"

#define NUM_SPARKS (NUM_LEDS / 2)

#ifndef PACIFICA_H
#define PACIFICA_H

#include <FastLED.h>
#include "config.h"
class Pacifica : public Pattern
{
    public:
        Pacifica(uint16_t _time, uint16_t _delay);
        ~Pacifica();

        void Generate(CRGB* arr);
        void Reset();
        void AddWhitecaps(CRGB* leds);
        void DeepenColors(CRGB* leds);
        void AddLayer(CRGB* leds, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
    private:

};

Pacifica::Pacifica(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) {
}

Pacifica::~Pacifica() {}

void Pacifica::Generate(CRGB* leds) 
{
        // Increment the four "color index start" counters, one for each wave layer.
        // Each is incremented at a different speed, and the speeds vary over time.
        static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
        static uint32_t sLastms = 0;
        uint32_t ms = GET_MILLIS();
        uint32_t deltams = ms - sLastms;
        sLastms = ms;
        uint16_t speedfactor1 = beatsin16(3, 179, 269);
        uint16_t speedfactor2 = beatsin16(4, 179, 269);
        uint32_t deltams1 = (deltams * speedfactor1) / 256;
        uint32_t deltams2 = (deltams * speedfactor2) / 256;
        uint32_t deltams21 = (deltams1 + deltams2) / 2;
        sCIStart1 += (deltams1 * beatsin88(1011,10,13));
        sCIStart2 -= (deltams21 * beatsin88(777,8,11));
        sCIStart3 -= (deltams1 * beatsin88(501,5,7));
        sCIStart4 -= (deltams2 * beatsin88(257,4,6));
        
        // Clear out the LED array to a dim background blue-green
        fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));
        
        // Render each of four layers, with different scales and speeds, that vary over time
        AddLayer(leds, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
        AddLayer( leds, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
        AddLayer( leds, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
        AddLayer( leds, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));
        
        // Add brighter 'whitecaps' where the waves lines up more
        AddWhitecaps(leds);
        
        // Deepen the blues and greens a bit
        DeepenColors(leds);
}

void Pacifica::Reset() {}


// Add one layer of waves into the led array
void Pacifica::AddLayer(CRGB* leds, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
      uint16_t ci = cistart;
      uint16_t waveangle = ioff;
      uint16_t wavescale_half = (wavescale / 2) + 20;
      
      for (uint16_t i = 0; i < NUM_LEDS; i++) {
            waveangle += 250;
            uint16_t s16 = sin16( waveangle ) + 32768;
            uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
            ci += cs;
            uint16_t sindex16 = sin16( ci) + 32768;
            uint8_t sindex8 = scale16( sindex16, 240);

            uint8_t hue = constrain(sindex8, Pattern::GetGlobalHue(), Pattern::GetGlobalHue() + 32);
            leds[i] += CHSV(hue, 255, bri);
      }
}

void Pacifica::AddWhitecaps(CRGB* leds)
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

void Pacifica::DeepenColors(CRGB* leds)
{
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        leds[i].blue = scale8( leds[i].blue,  145); 
        leds[i].green= scale8( leds[i].green, 200); 
        leds[i] |= CRGB( 2, 5, 7);
      }
}

#endif // PACIFICA_H
