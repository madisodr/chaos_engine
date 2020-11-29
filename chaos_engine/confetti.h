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
        uint8_t m_last_pos;
};

Confetti::Confetti(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) 
{
     m_last_pos = random8(NUM_LEDS);
}

Confetti::~Confetti() {}
void Confetti::Reset() {}

void Confetti::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 60);
    
    arr[m_last_pos] = Pattern::GetGlobalCHSV(50);
    uint8_t pos = random8(NUM_LEDS);

    arr[pos] = Pattern::GetGlobalCHSV();
    m_last_pos = pos;
}

#endif // CONFETTI_H
