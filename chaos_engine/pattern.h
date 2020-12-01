#ifndef PATTERN_H
#define PATTERN_H

#include <FastLED.h>
#include "utils.h"

class Pattern
{
    public:
        Pattern(uint16_t _time, uint16_t _delay);
        ~Pattern();

        virtual void Generate(CRGB* arr);
        virtual void Reset();

        uint16_t GetDelay() const;
        uint16_t GetTime() const;
        void ToggleReverse();

        static void UpdateGlobalHue();
        static uint8_t GetGlobalHue();
        static void SetGlobalHue(uint8_t _hue);
        static CHSV GetGlobalCHSV(uint8_t brightness, uint8_t saturation, uint8_t shift);
        static CRGB GetGlobalCRGB(uint8_t shift);
    protected:
        void Breath(float _speed);
        bool m_reverse;
    private:
        uint16_t m_delay;
        uint16_t m_time;
        static uint8_t s_hue;
};

uint8_t Pattern::s_hue;

Pattern::Pattern(uint16_t _time, uint16_t _delay) : m_time(_time), m_delay(_delay), m_reverse(false) {}
Pattern::~Pattern() {}

uint16_t Pattern::GetDelay() const
{
    return m_delay;
}

uint16_t Pattern::GetTime() const
{
    return m_time;
}

void Pattern::UpdateGlobalHue()
{
    s_hue++;
    s_hue %= 255;
}

uint8_t Pattern::GetGlobalHue()
{
    return s_hue;
}

CHSV Pattern::GetGlobalCHSV(uint8_t brightness = MAX_BRIGHTNESS, uint8_t saturation = 255, uint8_t shift = 0)
{
    return CHSV(s_hue + shift, 255, brightness);
}

CRGB Pattern::GetGlobalCRGB(uint8_t shift = 0)
{
    return wheel(s_hue + shift);
}

void Pattern::SetGlobalHue(uint8_t _hue)
{
    s_hue = _hue;
}

void Pattern::Breath(float _speed)
{
    float breath = (exp(sin(millis() / _speed * PI)) - 0.36787944) * 108.0;
    breath = map(breath, 0, MAX_BRIGHTNESS, 20, 80);
    FastLED.setBrightness(breath);
}

inline void Pattern::ToggleReverse()
{
    m_reverse ^= true;
}

#endif // PATTERN_H
