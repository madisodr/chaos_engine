#ifndef BREATHING_RAINBOW_H
#define BREATHING_RAINBOW_H

#include <FastLED.h>
#include "config.h"

class BreathingRainbow : public Pattern
{
    public:
        BreathingRainbow(uint16_t _time, uint16_t _delay);
        ~BreathingRainbow();

        void Generate(CRGB* arr);
    private:
        const byte m_delta = 255 / NUM_LEDS;
        uint8_t m_hue;
};

BreathingRainbow::BreathingRainbow(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) {}

BreathingRainbow::~BreathingRainbow() {}

void BreathingRainbow::Generate(CRGB* arr)
{
    Breath();

    // increase the hue by 1 each time
    m_hue++;

    m_num_leds = NUM_LEDS;

    CHSV hsv;
    hsv.hue = m_hue;
    hsv.val = 255;
    hsv.sat = 255;
    for( int i = 0; i < NUM_LEDS; i++) {
        arr[i] = hsv;
        hsv.hue += m_delta;
    }
}

#endif // BREATHING_RAINBOW_H
