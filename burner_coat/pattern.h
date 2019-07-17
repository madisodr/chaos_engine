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
        
        uint16_t GetDelay();
        uint16_t GetTime();
        void ToggleReverse();
    protected:
        void Breath(float _speed);
        bool m_reverse;
        uint16_t m_num_leds;
        uint8_t m_pixel_count;
    private:
        uint16_t m_delay;
        uint16_t m_time;
};

Pattern::Pattern(uint16_t _time, uint16_t _delay) : m_time(_time), m_delay(_delay), m_num_leds(1), m_reverse(false) {}

Pattern::~Pattern() {}

inline uint16_t Pattern::GetDelay()
{
    return m_delay;
}

inline uint16_t Pattern::GetTime()
{
    return m_time;
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
