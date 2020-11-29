#ifndef RAINBOWRIPPLE_H
#define RAINBOWRIPPLE_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class RainbowRipple : public Pattern
{
    public:
        RainbowRipple(uint16_t _time, uint16_t _delay);
        ~RainbowRipple();

        void Generate(CRGB* arr);
        void Reset();
    private:
        uint8_t m_center;
        int8_t m_step;
        uint8_t m_max_steps;
        float m_fade_rate;
        uint8_t m_color_multi;
};

RainbowRipple::RainbowRipple(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_step = -1;
    m_max_steps = 10;
    m_fade_rate = 0.9;
}

RainbowRipple::~RainbowRipple() {}
inline void RainbowRipple::Reset() 
{
    m_center = random(NUM_LEDS);
    m_step = -1;   
}

void RainbowRipple::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 64);

    if (m_step == -1) {
        m_center = random(NUM_LEDS);
        m_step = 0;
        m_color_multi = 5;
    }
    
    if (m_step == 0) {
        arr[m_center] = Pattern::GetGlobalCHSV();
        m_step++;
    } else { 
        if (m_step < m_max_steps) {
            float bri = pow(m_fade_rate, m_step) * MAX_BRIGHTNESS;
            arr[(m_center + m_step + NUM_LEDS) % NUM_LEDS] = wheel(GetGlobalHue() + m_color_multi); // wheel(m_color)
            arr[(m_center - m_step + NUM_LEDS) % NUM_LEDS] = wheel(GetGlobalHue() + m_color_multi);
    
            //arr[(m_center + (NUM_LEDS / random8(4)) + m_step + NUM_LEDS) % NUM_LEDS] = wheel(m_color_multi);
            //arr[(m_center + (NUM_LEDS / random8(4)) - m_step + NUM_LEDS) % NUM_LEDS] = wheel(m_color_multi);
            
            m_step++;
            m_color_multi += (256 / m_max_steps);
        } else {
            m_step = -1;
        }
    }
    
    blur1d(arr, NUM_LEDS, .1);
}

#endif // RAINBOWRIPPLE_H
