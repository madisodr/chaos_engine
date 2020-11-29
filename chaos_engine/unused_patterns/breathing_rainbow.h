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
        void Reset();
    private:
        byte m_delta = 255 / NUM_LEDS;
        uint8_t m_hue;
};

BreathingRainbow::BreathingRainbow(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) {}
BreathingRainbow::~BreathingRainbow() {}
inline void BreathingRainbow::Reset() {}

void BreathingRainbow::Generate(CRGB* leds)
{
    m_hue++;

    CHSV hsv = CHSV(m_hue, 255, 255);
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = hsv;
        hsv.hue += m_delta;
    }
}

#endif // BREATHING_RAINBOW_H
