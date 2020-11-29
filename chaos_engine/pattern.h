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

        // TODO Write a cleanup method that gets called when a pattern is no longer
        // in use.
        // virtual void Cleanup();

        uint16_t GetDelay() const;
        uint16_t GetTime() const;
        void ToggleReverse();

        static void UpdateGlobalHue();
        static uint8_t GetGlobalHue();
        static void SetGlobalHue(uint8_t _hue);
        static CHSV GetGlobalCHSV(uint8_t brightness, uint8_t saturation, uint8_t shift);

        static void Glitch(CRGB* leds);
        static void ResetGlitch();

        static bool m_isGlitching;
        static uint32_t m_nextGlitch;
        static uint8_t m_glitchFrames;
        static uint8_t m_glitchStrip;
        static uint32_t m_frame;
    protected:
        void Breath(float _speed);
        bool m_reverse;
    private:
        uint16_t m_delay;
        uint16_t m_time;
        static uint8_t s_hue;
};

uint8_t Pattern::s_hue;

bool Pattern::m_isGlitching = false;
uint32_t Pattern::m_nextGlitch = random(20);
uint8_t Pattern::m_glitchFrames = random(1, 10);
uint8_t Pattern::m_glitchStrip = random(100);
uint32_t Pattern::m_frame = 0;

Pattern::Pattern(uint16_t _time, uint16_t _delay) : m_time(_time), m_delay(_delay), m_reverse(false)
{
}

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

void Pattern::ResetGlitch()
{
    m_nextGlitch = m_frame + random(1, 20);
    m_glitchFrames = random(1, 10);
    m_isGlitching = false;
    m_glitchStrip = random(100);
}

// Random strip flicker
void Pattern::Glitch(CRGB* leds)
{
    if (m_frame >= m_nextGlitch) {
        m_isGlitching = true;
    }

    if (m_isGlitching) {
        int shift = random(4);
        if (m_frame >= m_nextGlitch && m_frame < m_nextGlitch + m_glitchFrames) {
            int start = NUM_LEDS; // * m_glitchStrip;
            int led_count = random(30);
            for (int i = start; i < start + led_count; ++i) {
                int index = (i + random(10)) % NUM_LEDS;
                CRGB thisColor = leds[i];

                if (random(100) < 40) {
                    thisColor += random(32);
                }

                leds[index] = thisColor;
            
            }
        } else if (m_frame >= m_nextGlitch + m_glitchFrames) {
            ResetGlitch();
        }
    }
}

#endif // PATTERN_H
