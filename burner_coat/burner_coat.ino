#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "playlist.h"

#include "breathing_rainbow.h"
#include "confetti.h"
#include "double_marqee.h"
//#include "noise.h"
#include "ripple.h"
//#include "shimmer.h"

/*uint8_t current_pattern = 0; // Index number of which pattern is current
CRGBPalette16 current_palette(orangePalette);
CRGBPalette16 target_palette;
*/
#define PATTERN_LENGTH 30

BreathingRainbow* breathing = new BreathingRainbow(PATTERN_LENGTH, 5);
Confetti* confetti = new Confetti(PATTERN_LENGTH, 10);
Ripple* ripple = new Ripple(PATTERN_LENGTH, 40);
DoubleMarqee* rainbow_marqee = new DoubleMarqee(PATTERN_LENGTH, 10);

Pattern* p_list[] = {
    rainbow_marqee,
    confetti,
    breathing,
    ripple,
};

Playlist* playlist = new Playlist(p_list, ARRAY_SIZE(p_list));

CRGB staging_a[NUM_LEDS];
CRGB staging_b[NUM_LEDS];
static uint8_t blend_amount = 0;
uint16_t start_blending = 0;
bool blending = false;
Pattern* running_pattern = playlist->Current();
Pattern* next_pattern = playlist->GetNextPattern();

/* setup */
void setup()
{
    //Serial.begin(19200);
    LEDS.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
    
    randomSeed(analogRead(A1));
    // Initialize our coordinates to some random values
#ifdef NOISE_H
    x = random16();
    y = random16();
    z = random16();
#endif
    // Set the maximum power the LEDs can pull
    set_max_power_in_volts_and_milliamps(5, MAX_VOLTS);
    start_blending = ((millis() / 1000)) + (PATTERN_LENGTH / 4) * 3;
}

/* main loop */
void loop()
{
    // Fetch current pattern
    uint16_t seconds = millis() / 1000;

    if (seconds >= start_blending)
        blending = true;

    running_pattern->Generate(staging_a);
    uint16_t _delay = running_pattern->GetDelay();

    if (blending) {
        next_pattern = playlist->GetNextPattern();
        next_pattern->Generate(staging_b);
        blend(staging_a, staging_b, leds, NUM_LEDS, blend_amount);

        EVERY_N_MILLISECONDS(10) {
            if (blending) {
                blend_amount += 1;
            }
        }

        if (blend_amount >= 255) {
            blend_amount = 255;
        }

        _delay += next_pattern->GetDelay();
    } else {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = staging_a[i];
        }
    }
    
    FastLED.show();
    FastLED.delay(_delay);

    EVERY_N_SECONDS_I(timer, running_pattern->m_time) {
        start_blending = ((millis() / 1000)) % 60 + (PATTERN_LENGTH / 4) * 3; // blend over 1/N the pattern time
        blending = false;
        blend_amount = 0;
        playlist->Seek();

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
