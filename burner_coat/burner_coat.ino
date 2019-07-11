#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "playlist.h"

#include "breathing_rainbow.h"

//#include "confetti.h"
#include "double_marqee.h"
// #include "juggle.h"
//#include "noise.h"
#include "ripple.h"
#include "shimmer.h"

uint8_t current_pattern = 0; // Index number of which pattern is current
CRGBPalette16 current_palette(orangePalette);
CRGBPalette16 target_palette;

BreathingRainbow* breathing = new BreathingRainbow(5);
Shimmer* shimmer = new Shimmer(5);
Ripple* ripple = new Ripple(5);
    
Pattern* p_list[] = { breathing, ripple};
Playlist* playlist = new Playlist(p_list, ARRAY_SIZE(p_list));
/* setup */
void setup() {
    LEDS.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);



    randomSeed(analogRead(A1));
    // Initialize our coordinates to some random values
#ifdef NOISE_H
    x = random16();
    y = random16();
    z = random16();
#endif
}

/* main loop */
void loop() {
    // A more power efficient way to show the LEDs
    set_max_power_in_volts_and_milliamps(5, MAX_VOLTS);
    show_at_max_brightness_for_power();

    // Fetch current pattern
    Pattern* running_pattern = (Pattern*)playlist->Current();
    running_pattern->Run();
    

    EVERY_N_SECONDS_I(timer, running_pattern->m_time) {
        playlist->Seek();

        fadeToBlackBy(leds, NUM_LEDS, 30);
        
        // Update timer period to new pattern's length
        timer.setPeriod(playlist->Current()->m_time);
    }

#ifdef NOISE_H
    // blend the current palette toward a new palette between 0 to 48 steps
    nblendPaletteTowardPalette(current_palette, target_palette, 24);

    // choose a new palette, speed, and scale
    updatePalette();

    // generate noise data
    generateNoise();
#endif
}

void updatePalette() {
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

inline CRGBPalette16 getCurrentPalette() {
    return current_palette;
}

