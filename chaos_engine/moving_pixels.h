#ifndef MOVING_PIXELS_H
#define MOVING_PIXELS_H

#include <FastLED.h>
#include "config.h"

#define MAX_PIXELS 8

class Pixel
{
    public:
        Pixel(bool reverse, bool use_alt_color);
        ~Pixel();
        void Update();
        bool m_reverse;
        bool m_is_running;
        uint8_t m_pos;
        bool m_alt_color;
};

Pixel::Pixel(bool reverse, bool use_alt_color)
{
    m_reverse = reverse;
    m_pos = m_reverse ? NUM_LEDS - 1 : 0;
    m_is_running = false;
    m_alt_color = use_alt_color;
}

Pixel::~Pixel()  {}

void Pixel::Update()
{
    m_reverse == true ? m_pos-- : m_pos++;
}

class MovingPixels : public Pattern
{
    public:
        MovingPixels(uint16_t _delay);
        ~MovingPixels();

        void Generate(CRGB* arr);
        void Reset();
        int8_t FindFirstStoppedPixel();
    private:
        uint8_t m_pixel_count;
        uint8_t m_pixel_freq;
        Pixel* m_pixels[MAX_PIXELS] ;
};

MovingPixels::MovingPixels(uint16_t _delay) : Pattern(_delay)
{
    m_pixel_freq = 10; // 10%
    for (int i = 0; i < MAX_PIXELS; i++) {
        m_pixels[i] = new Pixel(random(2), (i % 2));
    }
}

void MovingPixels::Reset() 
{
}

MovingPixels::~MovingPixels() {}

int8_t MovingPixels::FindFirstStoppedPixel()
{
    for (int i = 0; i < MAX_PIXELS; i++) {
        if (!m_pixels[i]->m_is_running) {
            return i;
        }
    }

    return -1;
}

void MovingPixels::Generate(CRGB* leds)
{
    fadeToBlackBy(leds, NUM_LEDS, 128);

    int new_pixel_index = FindFirstStoppedPixel();
    if (new_pixel_index != -1 && (random(255) < m_pixel_freq)) {
        Pixel* p = m_pixels[new_pixel_index];
        
        if (p != NULL) {
            p->m_is_running = true;
            p->m_pos = (p->m_reverse == true) ? (NUM_LEDS - 1) : 0;
        }
    }
    
    for (int i = 0; i < MAX_PIXELS; i++) {
        Pixel* p = m_pixels[i];
        p->Update();
        
        if (p->m_pos < 0 || p->m_pos >= NUM_LEDS) {
            p->m_is_running = false;
            p->m_reverse = false;
        }
        
        if (!p->m_is_running) { 
            continue;
        } else {
            if (p->m_reverse == true) {
                leds[p->m_pos] = Pattern::GetGlobalCRGB(85);
            } else {
                leds[p->m_pos] = Pattern::GetGlobalCRGB();
            }
        }
    }
}

#endif // MOVING_PIXELS_H
