#ifndef PATTERN_H
#define PATTERN_H

struct CRGB;

class Pattern
{
    public:
        Pattern(uint16_t _time, uint16_t _delay);
        ~Pattern();

        virtual void Generate(CRGB* arr);
        uint16_t GetDelay();
        void Reset();

        uint16_t m_time;
        uint8_t m_max_brightness;
        uint16_t m_num_leds;
        
    protected:
        void Breath();
    private:
        uint16_t m_delay;
};

Pattern::Pattern(uint16_t _time, uint16_t _delay) : m_time(_time), m_num_leds(1), m_delay(_delay) {}

Pattern::~Pattern() {}

uint16_t Pattern::GetDelay()
{
    return m_delay;
}

void Pattern::Reset()
{
    m_num_leds = 0;
}

void Pattern::Breath()
{
    float breath = (exp(sin(millis() / 5000.0 * PI)) - 0.36787944) * 108.0;
    breath = map(breath, 0, MAX_BRIGHTNESS, 20, 80);
    FastLED.setBrightness(breath);
}

#endif // PATTERN_H
