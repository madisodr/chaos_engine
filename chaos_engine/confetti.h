#ifndef CONFETTI_H
#define CONFETTI_H

#include <FastLED.h>
#include "config.h"
#include "utils.h"

class Confetti : public Pattern
{
    public:
        Confetti(uint16_t _delay);
        ~Confetti();
        void Generate(CRGB* arr);
        void Reset();
    private:
        bool m_alt_color;
};

Confetti::Confetti(uint16_t _delay) : Pattern(_delay) 
{
    m_alt_color = false;
}
Confetti::~Confetti() {}

void Confetti::Reset() {}

void Confetti::Generate(CRGB* leds)
{
    fadeToBlackBy(leds, NUM_LEDS, 60);
    if (m_alt_color) {
        leds[random(NUM_LEDS)] = Pattern::GetGlobalCRGB(85);
    } else {
        leds[random(NUM_LEDS)] = Pattern::GetGlobalCRGB();
    }
     m_alt_color = !m_alt_color;
}

#endif // CONFETTI_H
