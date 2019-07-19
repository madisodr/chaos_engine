#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "playlist.h"

#include "breathing_rainbow.h"
#include "fire.h"
#include "confetti.h"
#include "double_marqee.h"
#include "moving_pixels.h"
#include "noise.h"
#include "ripple.h"

/* lol. Ugly but whatever. If you want to add a pattern, add a pointer to it here. */
Pattern* p_list[] = {
    new Fire(PATTERN_LENGTH, 0),
    new Ripple(PATTERN_LENGTH, 50),
    new Noise(PATTERN_LENGTH, 60),
    new MovingPixels(PATTERN_LENGTH, 40),
    new BreathingRainbow(PATTERN_LENGTH, 10),
    new DoubleMarqee(PATTERN_LENGTH, 20),
    new Confetti(PATTERN_LENGTH, 10),
};

Playlist* playlist;

Pattern* running_pattern;
Pattern* next_pattern;

// The leds
CRGB leds[NUM_LEDS];
CRGB staging[NUM_LEDS];

float blend_amount;
uint16_t start_blending;
bool blending;

/* setup */
void setup()
{
    LEDS.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    randomSeed(analogRead(A1));

    // Set the maximum power the LEDs can pull
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_VOLTS);

    playlist = new Playlist(p_list, ARRAY_SIZE(p_list));

    running_pattern = playlist->GetCurrent();

    playlist->SetupNextPattern(false);
    next_pattern = playlist->GetNext();
    playlist->SetTotalDelay(running_pattern->GetDelay());

    blend_amount = 0.0;
    start_blending = millis() + (running_pattern->GetTime() * (BLEND_TIME_MULTIPLIER / 3));
    blending = false;
}

/* main loop */
void loop()
{
    // generate the current pattern
    running_pattern->Generate(leds);

    // if the timer start_blending has elapsed, flip the flag
    if (millis() >= start_blending) {
        blending = true;
    }

    if (blending && next_pattern != NULL) {
        // generate and blend the next pattern into the original
        next_pattern->Generate(staging);
        blend(leds, staging, leds, NUM_LEDS, blend_amount);

        // Adjust the blend amount of the second pattern into the first.
        EVERY_N_MILLISECONDS(int((running_pattern->GetTime() * BLEND_TIME_MULTIPLIER) / 255)) {
            if (blend_amount < 255) {
                blend_amount++;
            }
        }

        // delay drift so the delay between patterns changes over smoothly
        EVERY_N_MILLISECONDS(DELAY_DRIFT) {
            int total_delay = playlist->GetTotalDelay();
            if (total_delay <= next_pattern->GetDelay()) {
                playlist->SetTotalDelay(total_delay + 1);
            } else {
                playlist->SetTotalDelay(total_delay - 1);
            }
        }
    }

    FastLED.show();
    FastLED.delay(playlist->GetTotalDelay());

    EVERY_N_SECONDS_I(timer, running_pattern->GetTime()) {
        // reset blending for new pattern
        start_blending = millis() + (running_pattern->GetTime() * (BLEND_TIME_MULTIPLIER / 3));
        blend_amount = 0;
        blending = false;

        playlist->SetupNextPattern(false);

        running_pattern = next_pattern;
        playlist->SetCurrentPattern(next_pattern);

        next_pattern = playlist->GetNext();
        next_pattern->Reset();

        // Update timer period to new pattern's length
        timer.setPeriod(running_pattern->GetTime());
    }
}
