#ifndef HELIOS_H
#define HELIOS_H

#include <FastLED.h>
#include "config.h"
class Helios : public Pattern
{
    public:
        Helios(uint16_t _delay);
        ~Helios();

        void Generate(CRGB* arr);
        void Reset();
        void CreateHeliosBuffer(bool* buffer);
    private:
        int offset;
        bool forward_buffer[NUM_LEDS];
        bool reverse_buffer[NUM_LEDS];
};

Helios::Helios(uint16_t _delay) : Pattern(_delay)
{
    offset = 1;
    CreateHeliosBuffer(forward_buffer);
    CreateHeliosBuffer(reverse_buffer);
}

Helios::~Helios() {}

void Helios::Generate(CRGB* leds)
{
    fadeToBlackBy(leds, NUM_LEDS, 255);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        int r = NUM_LEDS - i;
        int forwardOffset = (i + offset) % NUM_LEDS;
        int reverseOffset = (r + offset) % NUM_LEDS;
        bool forward = forward_buffer[forwardOffset];
        bool reverse = reverse_buffer[reverseOffset];

        //CRGB color;

        if (forward) {
            // int b = random(16);
            // color = Pattern::GetGlobalCRGB(b);
            leds[i] = Pattern::GetGlobalCRGB(random(16));
        }

        if (reverse) {
            // color = Pattern::GetGlobalCRGB(random(16));
            leds[i] = Pattern::GetGlobalCRGB(random(16));
        }
    }

    offset = (offset + 1) % NUM_LEDS;
}

void Helios::Reset() {}

void Helios::CreateHeliosBuffer(bool* buffer)
{
    uint8_t forwardOdds = 128;  // Chance on will flip to off. [0-255]
    uint8_t reverseOdds = 32;
    bool flip = true;
    for (int i = 0; i < NUM_LEDS; i++) {
        buffer[i] = flip;

        if (flip) {
            if (random(255) < forwardOdds) {
                flip = !flip;
            }
        } else {
            if (random(255) < reverseOdds) {
                flip = !flip;
            }
        }
    }
}

#endif // HELIOS_H
