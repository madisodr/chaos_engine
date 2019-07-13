#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "playlist.h"

#include "breathing_rainbow.h"
#include "confetti.h"
#include "double_marqee.h"
#include "noise.h"
#include "ripple.h"
//#include "shimmer.h"

#define PATTERN_LENGTH 20

BreathingRainbow* breathing = new BreathingRainbow(PATTERN_LENGTH, 5);
Confetti* confetti = new Confetti(PATTERN_LENGTH, 10);
Ripple* ripple = new Ripple(PATTERN_LENGTH, 40);
DoubleMarqee* rainbow_marqee = new DoubleMarqee(PATTERN_LENGTH, 10);
Noise* noise = new Noise(PATTERN_LENGTH, 30);

Pattern* p_list[] = {
    rainbow_marqee,
    noise,
    confetti,
    breathing,
    ripple,
};

Playlist* playlist = new Playlist(p_list, ARRAY_SIZE(p_list));

CRGB staging_a[NUM_LEDS];
CRGB staging_b[NUM_LEDS];

static float blend_amount = 0;
static uint16_t start_blending = 0;
bool blending = false;

Pattern* running_pattern = playlist->GetPattern(playlist->Current());
Pattern* next_pattern = playlist->GetPattern(playlist->GetNextPattern(false));

/* setup */
void setup()
{
    //Serial.begin(19200);
    LEDS.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);

    randomSeed(analogRead(A1));
    // Initialize our coordinates to some random values
#ifdef NOISE_H
    Noise::x = random16();
    Noise::y = random16();
    Noise::z = random16();
#endif
    // Set the maximum power the LEDs can pull
    set_max_power_in_volts_and_milliamps(5, MAX_VOLTS);
    start_blending = millis() + (PATTERN_LENGTH * 1000L / 4 * 3);
}

/* main loop */
void loop()
{
    // Fetch current pattern
    running_pattern->Generate(staging_a);
    uint16_t _delay = running_pattern->GetDelay();

    if (millis() >= start_blending) {
        blending = true;
    }

    if (blending && next_pattern != NULL) {
        next_pattern->Generate(staging_b);
        blend(staging_a, staging_b, leds, NUM_LEDS, blend_amount);

        EVERY_N_MILLISECONDS(int((running_pattern->m_time * 1000) / 255)) {
            if (blending && blend_amount < 255) {
                blend_amount++;
            }
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
        start_blending = millis() + (PATTERN_LENGTH * 1000L / 4 * 3);
        blend_amount = 0;
        blending = false;

        uint8_t next_pattern_index = playlist->GetNextPattern(true);
        
        running_pattern = next_pattern;
        next_pattern = playlist->GetPattern(next_pattern_index);
        next_pattern->Reset();
        // Update timer period to new pattern's length
        timer.setPeriod(running_pattern->m_time);
    }

#ifdef NOISE_H

#endif
}
