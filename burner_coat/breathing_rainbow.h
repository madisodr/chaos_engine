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
    //Breath(5000.0);
    m_hue++;

    CHSV hsv = CHSV(m_hue, 255, MAX_BRIGHTNESS);
    for ( int i = 0; i < NUM_LEDS; i++) {
        arr[i] = hsv;
        hsv.hue += m_delta;
    }
}

#endif // BREATHING_RAINBOW_H
