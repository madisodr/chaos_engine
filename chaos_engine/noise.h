#ifndef NOISE_H
#define NOISE_H

#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "utils.h"

#define NOISE_SPEED 40
#define NOISE_SCALE 70

#define SPEED_STEP 2


class Noise : public Pattern
{
    public:
        Noise(uint16_t _time, uint16_t _delay);
        ~Noise();

        void Generate(CRGB* arr);
        uint16_t XY(uint8_t x, uint8_t y);
        void MakeNoise();
        void UpdateSpeed();
        void Reset();


    private:
        // Speed of the pattern
        uint8_t m_speed;

        // Scale determines how far apart the pixels in our noise matrix are.
        uint8_t m_scale;

        // This is the array that we keep our computed noise values in
        uint8_t m_noise[NUM_LEDS];

        CRGB m_color;

        // Override for the global max brightness
        static const uint8_t MAX_BRIGHTNESS_OVERRIDE = 0;
        
        // The 16 bit version of̈ our coordinates. Should probably be private but
        uint16_t m_x;
};

Noise::Noise(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_speed = NOISE_SPEED;
    m_scale = NOISE_SCALE;
    m_x = random16();
}

Noise::~Noise() {}

void Noise::Reset() {}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void Noise::MakeNoise()
{
    // If we're runing at a low "speed", some 8-bit artifacts become visible
    // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
    // The amount of data smoothing we're doing depends on "speed".
    uint8_t dataSmoothing = (m_speed < 50) ? (200 - (m_speed * 4)) : 0;
    

    for (int i = 0; i < NUM_LEDS; i++) {
        int ioffset = m_scale * i;
        uint8_t data = inoise8(m_x + ioffset);

        if (dataSmoothing) {
            uint8_t oldData = m_noise[i];
            uint8_t newData = scale8(oldData, dataSmoothing) + scale8(data, 256 - dataSmoothing);
            data = newData;
        }

        m_noise[i] = data;
    
    }

    // apply slow drift to X and Y, just for visual variation.
    m_x += m_speed / 16;
}

// heavily modified version of https://github.com/FastLED/FastLED/blob/master/examples/NoisePlusPalette/NoisePlusPalette.ino
void Noise::Generate(CRGB* leds)
{
    UpdateSpeed();

    // generate noise data
    MakeNoise();
    
    for (int i = 0; i < NUM_LEDS; i++) {
        
        uint8_t index = m_noise[i];
        uint8_t bri =   m_noise[i];

        // brighten up, as the color palette itself often contains the
        // light/dark dynamic range desired
        if (bri > 127) {
            bri = 127;
        } else {
            bri = dim8_raw(bri * 2);
        }

        leds[i] = Pattern::GetGlobalCHSV(bri, 255, random8(8)); //CHSV(Pattern::GetGlobalHue() + random8(8), 255, bri);
    }
}

void Noise::UpdateSpeed()
{
    uint8_t second_hand = (millis() / 1000) % 60;
    static uint8_t last_second = 99;
    
    EVERY_N_SECONDS(1) {
        (second_hand < 30) ? (m_speed += SPEED_STEP) : (m_speed -= SPEED_STEP);
    }
}

#endif
