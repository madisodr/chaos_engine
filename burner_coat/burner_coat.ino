#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "playlist.h"

#include "breathing_rainbow.h"
#include "confetti.h"
#include "double_marqee.h"
//#include "noise.h"
#include "ripple.h"
#include "moving_pixels.h"
//#include "shimmer.h"

#define PATTERN_LENGTH 30

BreathingRainbow* breathing = new BreathingRainbow(PATTERN_LENGTH, 10);
Confetti* confetti = new Confetti(PATTERN_LENGTH, 10);
Ripple* ripple = new Ripple(PATTERN_LENGTH, 50);
DoubleMarqee* rainbow_marqee = new DoubleMarqee(PATTERN_LENGTH, 20);
MovingPixels* pixels = new MovingPixels(PATTERN_LENGTH, 30);

Pattern* p_list[] = {
    breathing,
    rainbow_marqee,
    pixels,
    confetti,
    ripple,
};

Playlist* playlist = new Playlist(p_list, ARRAY_SIZE(p_list));

CRGB staging_a[NUM_LEDS];
CRGB staging_b[NUM_LEDS];

float blend_amount;
uint16_t start_blending;
bool blending = false;

Pattern* running_pattern;
Pattern* next_pattern;

#define BLEND_TIME_MULTIPLIER 1000L / 4 * 3

/* setup */
void setup()
{
    LEDS.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

    randomSeed(analogRead(A1));

    // Set the maximum power the LEDs can pull
    set_max_power_in_volts_and_milliamps(5, MAX_VOLTS);

    running_pattern = playlist->GetCurrent();

    playlist->SetupNextPattern(true);
    next_pattern = playlist->GetNext();
    playlist->SetTotalDelay(running_pattern->GetDelay());

    start_blending = millis() + min((running_pattern->GetTime() * BLEND_TIME_MULTIPLIER), 20);
    blending = false;
    blend_amount = 0.0;
}

/* main loop */
void loop()
{
    // Fetch current pattern
    running_pattern->Generate(staging_a);

    if (millis() >= start_blending) {
        blending = true;
    }

    if (blending && next_pattern != NULL) {
        next_pattern->Generate(staging_b);
        blend(staging_a, staging_b, leds, NUM_LEDS, blend_amount);

        EVERY_N_MILLISECONDS(int((running_pattern->GetTime() * 1000) / 255)) {
            if (blend_amount < 255) {
                blend_amount++;
            }
        }

        EVERY_N_MILLISECONDS(500) {
            if (playlist->GetTotalDelay() < next_pattern->GetDelay()) {
                playlist->SetTotalDelay(playlist->GetTotalDelay() + 1);
            } else if (playlist->GetTotalDelay() > next_pattern->GetDelay()) {
                playlist->SetTotalDelay(playlist->GetTotalDelay() - 1);
            }
        }
    } else {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = staging_a[i];
        }
    }

    FastLED.show();
    FastLED.delay(playlist->GetTotalDelay());

    EVERY_N_SECONDS_I(timer, running_pattern->GetTime()) {
        start_blending = millis() + min((running_pattern->GetTime() * BLEND_TIME_MULTIPLIER), 20);
        blend_amount = 0;
        blending = false;

        playlist->SetupNextPattern(true);

        running_pattern = next_pattern;
        playlist->SetCurrentPattern(next_pattern);

        next_pattern = playlist->GetNext();

        // Update timer period to new pattern's length
        timer.setPeriod(running_pattern->GetTime());
    }
}
