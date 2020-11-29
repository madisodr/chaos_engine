#ifndef MOVING_PIXELS_H
#define MOVING_PIXELS_H

#include <FastLED.h>
#include "config.h"

#define MAX_PIXELS 20

class Pixel
{
    public:
        Pixel(bool reverse);
        ~Pixel();
        void Update();
        bool m_reverse;
        bool m_isRunning;
        uint8_t m_pos;
};

Pixel::Pixel(bool reverse)
{
    m_reverse = reverse;
    m_pos = m_reverse ? NUM_LEDS - 1 : 0;
    m_isRunning = false;
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
        int8_t FindFirstStoppedPixel();
    private:
        uint8_t m_pixel_count;
        uint8_t m_pixel_freq;
        Pixel* m_pixels[MAX_PIXELS] ;
};

MovingPixels::MovingPixels(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_pixel_freq = 26; // 10%
    for (int i = 0; i < MAX_PIXELS; i++) {
        m_pixels[i] = new Pixel((random() % 2));
    }
}

void MovingPixels::Reset() 
{
}

MovingPixels::~MovingPixels() {}

int8_t MovingPixels::FindFirstStoppedPixel() {
    for (int i = 0; i < MAX_PIXELS; i++) {
        Pixel* p = m_pixels[i];
        if (p->m_isRunning == false) {
            return i;
        }
    }

    return -1;
}

void MovingPixels::Generate(CRGB* leds)
{
    fadeToBlackBy(leds, NUM_LEDS, 64);

    int new_pixel_index = FindFirstStoppedPixel();
    if (new_pixel_index != -1 && (random(255) < m_pixel_freq)) {
        Pixel* p = m_pixels[new_pixel_index];
        
        if (p != NULL) {
            p->m_isRunning = true;
            p->m_pos = (p->m_reverse == true) ? (NUM_LEDS - 1) : 0;
        }
    }
    
    for (int i = 0; i < MAX_PIXELS; i++) {
        Pixel* p = m_pixels[i];
        p->Update();
        
        if (p->m_pos < 0 || p->m_pos >= NUM_LEDS) {
            p->m_isRunning = false;
            p->m_reverse = (random() % 2);
        }
        
        if (!p->m_isRunning) { 
            continue;
        } else {
            leds[p->m_pos] = Pattern::GetGlobalCHSV();
        }
    }
}

#endif // MOVING_PIXELS_H
