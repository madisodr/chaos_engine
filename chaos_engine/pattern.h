#ifndef PATTERN_H
#define PATTERN_H

#include <FastLED.h>
struct CRGB;

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
        
        static bool blending;
        static float blend_amount;
             
    protected:
        void Breath(float _speed);
        bool m_reverse;
    private:
        uint16_t m_delay;
        uint16_t m_time;
        static uint8_t s_hue;   
};

bool Pattern::blending = false;
uint8_t Pattern::s_hue = 0;
float Pattern::blend_amount = 0.0;

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
