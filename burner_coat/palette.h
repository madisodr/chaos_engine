#ifndef PALETTE_H
#define PALETTE_H

uint8_t current_pattern = 0; // Index number of which pattern is current
CRGBPalette16 current_palette(orangePalette);
CRGBPalette16 target_palette;

void updatePalette()
{
    uint8_t hold_palette_multiplier = 1;
    uint8_t speed_step = 1;
    static uint8_t palette_idx = Palette_Ids::MIX; // index for the palettes array
    uint8_t second_hand = ((millis() / 1000) / hold_palette_multiplier) % 60;
    static uint8_t last_second = 99;

    const CRGBPalette16 palettes[2] = {mixedPalette, orangePalette};

    if (last_second != second_hand) {
        last_second = second_hand;
#ifdef NOISE_H
        if (0 == second_hand % 10) {
            second_hand < 30 ? speed += speed_step : speed -= speed_step;
            palette_idx++;
        }
#endif

        // wrap the palette index around to 0
        palette_idx %= ARRAY_SIZE(palettes);
        target_palette = palettes[palette_idx];
    }
}

inline CRGBPalette16 getCurrentPalette()
{
    return current_palette;
}

#endif // PALETTE_H
