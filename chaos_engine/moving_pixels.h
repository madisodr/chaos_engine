#ifndef MOVING_PIXELS_H
#define MOVING_PIXELS_H

#include <FastLED.h>
#include "config.h"
#include <QueueList.h>

#define MAX_PIXELS 5

class Pixel
{
    public:
        Pixel(bool reverse);
        ~Pixel();
        void Update();
        bool m_reverse;
        bool m_mark_for_cleanup;
        uint8_t m_pos;
};

Pixel::Pixel(bool reverse)
{
    m_reverse = reverse;
    m_pos = m_reverse ? NUM_LEDS - 1 : 0;
    m_mark_for_cleanup = false;
}

Pixel::~Pixel()  {}

void Pixel::Update()
{
    m_reverse == true ? m_pos-- : m_pos++;
}

class MovingPixels : public Pattern
{
    public:
        MovingPixels(uint16_t _time, uint16_t _delay);
        ~MovingPixels();

        void Generate(CRGB* arr);
        void Reset();
    private:
        uint8_t m_pixel_count;
        float m_pixel_freq;
        QueueList<Pixel*> m_pixels;
};

MovingPixels::MovingPixels(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_pixel_freq = 0.05;
}

void MovingPixels::Reset() 
{
    // Make sure we clear this out on reset. This should happen in a "cleanup"
    // stage after this pattern finishes, not before it starts again.
     while (!m_pixels.isEmpty()) {
        delete m_pixels.pop();
     }
}

MovingPixels::~MovingPixels() {}

void MovingPixels::Generate(CRGB* leds)
{
    fadeToBlackBy(leds, NUM_LEDS, 128);
    
    if (m_pixels.count() < MAX_PIXELS && (RandomFloat() < m_pixel_freq)) {
        Pixel* p = new Pixel(random() % 2);
        m_pixels.push(p);
    }
    
    for (int i = 0; i < m_pixels.count(); i++) {
        Pixel* p = m_pixels[i];
        
        if (p == NULL) { 
            break; 
        }
        
        p->Update();
        
        if (p->m_pos < 0 || p->m_pos >= NUM_LEDS) {
            p->m_mark_for_cleanup = true;
        } else {
            leds[p->m_pos] = Pattern::GetGlobalCHSV();
        }
    }
    
    while (!m_pixels.isEmpty() && m_pixels.peek()->m_mark_for_cleanup) {
        m_pixels.pop();
    }
}

#endif // MOVING_PIXELS_H
