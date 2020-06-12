#include <FastLED.h>
#include "config.h"

#include "pattern.h"
#include "playlist.h"

#include "confetti.h"
//#include "chaos_engine.h"
#include "noise.h"
#include "ripple.h"
#include "moving_pixels.h"
#include "double_marqee.h"

/* lol. Ugly but whatever. If you want to add a pattern, add a pointer to it here. */
Pattern* p_list[] = {
    //new Ripple(PATTERN_LENGTH, 70),
    //new Confetti(PATTERN_LENGTH,60),
    //new Noise(PATTERN_LENGTH, 50),
    //new MovingPixels(PATTERN_LENGTH, 70),
    new DoubleMarqee(PATTERN_LENGTH, 50),
    //new ChaosEngine(PATTERN_LENGTH, 20),   
};

Playlist* playlist;

// The leds
CRGB leds[NUM_LEDS];
CRGB staging[NUM_LEDS];

uint16_t start_blending;

/* setup */
void setup()
{
    LEDS.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    randomSeed(analogRead(1));
    
    Pattern::SetGlobalHue(random8(255));

    // Set the maximum power the LEDs can pull
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_VOLTS);

    playlist = new Playlist(p_list, ARRAY_SIZE(p_list));
    playlist->SetupNextPattern(true);
    playlist->SetTotalDelay(playlist->GetCurrent()->GetDelay());

    Pattern::blend_amount = 0.0;
    start_blending = millis() + (playlist->GetCurrent()->GetTime() * BLEND_TIME_MULTIPLIER);
    Pattern::blending = false;
}

/* main loop */
void loop()
{
    Pattern* running_pattern = playlist->GetCurrent();
    Pattern* next_pattern = playlist->GetNext();
    // generate the current pattern
    running_pattern->Generate(leds);

    EVERY_N_MILLISECONDS(1000) {
        Pattern::UpdateGlobalHue();
    }

    // if the timer start_blending has elapsed, flip the flag
    if (millis() >= start_blending) {
        Pattern::blending = true;
    }

    if (Pattern::blending && next_pattern != NULL) {
        // generate and blend the next pattern into the original
        next_pattern->Generate(staging);
        blend(leds, staging, leds, NUM_LEDS, Pattern::blend_amount);

        // Adjust the blend amount of the second pattern into the first.
        EVERY_N_MILLISECONDS(int((running_pattern->GetTime() * BLEND_TIME_MULTIPLIER) / 255)) {
            if (Pattern::blend_amount < 255) {
                Pattern::blend_amount += 1;
            }
        }

        // delay drift so the delay between patterns changes over smoothly
        EVERY_N_MILLISECONDS(DELAY_DRIFT) {
            int total_delay = playlist->GetTotalDelay();
            if (total_delay < next_pattern->GetDelay()) {
                playlist->SetTotalDelay(total_delay + 1);
            } else if (total_delay > next_pattern->GetDelay()) {
                playlist->SetTotalDelay(total_delay - 1);
            }
        }
    }

    FastLED.show();
    FastLED.delay(playlist->GetTotalDelay());
  
    EVERY_N_SECONDS_I(timer, running_pattern->GetTime()) {
        // reset blending for new pattern
        start_blending = millis() + (running_pattern->GetTime() * BLEND_TIME_MULTIPLIER);
        Pattern::blend_amount = 0;
        Pattern::blending = false;

        playlist->SetupNextPattern(true);

        running_pattern = next_pattern;
        playlist->SetCurrentPattern(next_pattern);

        next_pattern = playlist->GetNext();
        next_pattern->Reset();

        // Update timer period to new pattern's length
        timer.setPeriod(running_pattern->GetTime());
    }
}
