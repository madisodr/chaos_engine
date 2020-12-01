#ifndef CONFETTI_H
#define CONFETTI_H

#include <FastLED.h>
#include "config.h"
#include "utils.h"

class Confetti : public Pattern
{
    public:
        Confetti(uint16_t _time, uint16_t _delay);
        ~Confetti();
        void Generate(CRGB* arr);
        void Reset();
    private:
};

Confetti::Confetti(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) {}
Confetti::~Confetti() {}

void Confetti::Reset() {}

void Confetti::Generate(CRGB* leds)
{
    fadeToBlackBy(leds, NUM_LEDS, 60);
    leds[random8(NUM_LEDS)] = Pattern::GetGlobalCRGB();
}

#endif // CONFETTI_H
