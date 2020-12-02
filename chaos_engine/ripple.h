#ifndef RIPPLE_H
#define RIPPLE_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class Ripple : public Pattern
{
    public:
        Ripple(uint16_t _delay);
        ~Ripple();

        void Generate(CRGB* arr);
        void Reset();
    private:
        uint8_t m_center;
        int8_t m_step;
        uint8_t m_max_steps;
        float m_fade_rate;
};

Ripple::Ripple(uint16_t _delay) : Pattern(_delay)
{
    m_center = random(NUM_LEDS);
    m_step = -1;
    m_max_steps = 15;
    m_fade_rate = 0.9;
}

Ripple::~Ripple() {}

void Ripple::Reset() 
{
    m_center = random(NUM_LEDS);
    m_step = -1;
    m_max_steps = 15;    
}

void Ripple::Generate(CRGB* arr)
{
    fadeToBlackBy(arr, NUM_LEDS, 64);
    
    if (m_step == -1) {
        m_center = random(NUM_LEDS);
        m_step = 0;
    }
    
    if (m_step == 0) {
        arr[m_center] = Pattern::GetGlobalCRGB();
        m_step++;
    } else { 
        if (m_step < m_max_steps) {
            float bri = pow(m_fade_rate, m_step) * MAX_BRIGHTNESS;
            arr[(m_center + m_step + NUM_LEDS) % NUM_LEDS] = Pattern::GetGlobalCRGB().subtractFromRGB(255 - bri); //Pattern::GetGlobalCHSV(bri); // wheel(m_color)
            arr[(m_center - m_step + NUM_LEDS) % NUM_LEDS] = Pattern::GetGlobalCRGB().subtractFromRGB(255 - bri);
    
            arr[(m_center + (NUM_LEDS / random(4)) + m_step + NUM_LEDS) % NUM_LEDS] = Pattern::GetGlobalCRGB().subtractFromRGB(bri); 
            arr[(m_center + (NUM_LEDS / random(4)) - m_step + NUM_LEDS) % NUM_LEDS] = Pattern::GetGlobalCRGB().subtractFromRGB(bri); 
            
            m_step++;
        } else {
            m_step = -1;
        }
    }
    
    blur1d(arr, NUM_LEDS, .1);
}

#endif // RIPPLE_H
