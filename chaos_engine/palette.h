#ifndef PALETTE_H
#define PALETTE_H

#include "config.h"

// ID's for each palette in our program
enum Palette_Ids {
    MIX,
    ORG
};


static uint8_t palette_speed = 10;
static uint8_t current_pattern = 0; // Index number of which pattern is current
CRGBPalette16 current_palette(OceanColors_p);
CRGBPalette16 target_palette;

void updatePalette()
{
    const uint8_t speed_step = 1;
    static uint8_t palette_idx = Palette_Ids::MIX; // index for the palettes array
    uint8_t second_hand = (millis() / 1000) % 60;
    static uint8_t last_second = 99;

    const CRGBPalette16 palettes[] = {ForestColors_p, OceanColors_p};

    if (last_second != second_hand) {
        last_second = second_hand;
        
        EVERY_N_SECONDS(5) {
            (second_hand < 30) ? (palette_speed += speed_step) : (palette_speed -= speed_step);
        }

        EVERY_N_SECONDS(30) {
            palette_idx++;
        }
        
        // wrap the palette index around to 0
        palette_idx %= ARRAY_SIZE(palettes);
        target_palette = palettes[palette_idx];
    }
}

inline CRGBPalette16 getCurrentPalette()
{
    return current_palette;
}

inline uint8_t GetCurrentSpeed()
{
    return palette_speed;
}

#endif // PALETTE_H
