#ifndef PATTERN_NAME_H
#define PATTERN_NAME_H

#include <FastLED.h>
#include "config.h"
class PatternName : public Pattern
{
    public:
        PatternName(uint16_t _delay);
        ~PatternName();

        void Generate(CRGB* leds);
        void Reset();
    private:
};

PatternName::PatternName(uint16_t _delay) : Pattern(_delay) {}

PatternName::~PatternName() {}

void PatternName::Generate(CRGB* leds) {}

void PatternName::Reset() {}


#endif // PATTERN_NAME_H
