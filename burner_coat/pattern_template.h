#ifndef PATTERN_NAME_H
#define PATTERN_NAME_H

#include <FastLED.h>
#include "config.h"
class PatternName : public Pattern
{
    public:
        PatternName(uint16_t _time, uint16_t _delay);
        ~PatternName();

        void Generate(CRGB* arr);
    private:
};

PatternName::PatternName(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay) {}

PatternName::~PatternName() {}

void PatternName::Generate(CRGB* arr) {}


#endif // PATTERN_NAME_H
