 #include <FastLED.h>
 
#include "config.h"
#include "chaos_engine.h"

#include "pattern.h"

ChaosEngine* chaos_engine;

// The leds
CRGB leds_A[NUM_LEDS];
CRGB leds_B[NUM_LEDS];
CRGB leds_C[NUM_LEDS];

/* setup */
void setup()
{
    LEDS.addLeds<WS2812B, LED_PIN_A, GRB>(leds_A, NUM_LEDS);
    LEDS.addLeds<WS2812B, LED_PIN_B, GRB>(leds_B, NUM_LEDS);
    LEDS.addLeds<WS2812B, LED_PIN_C, GRB>(leds_C, NUM_LEDS);
    
    randomSeed(millis() + analogRead(0));
    
    // Set the maximum power the LEDs can pull
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_VOLTS);
    chaos_engine = new ChaosEngine();
    
    chaos_engine->RegisterPattern(ChaosEngine::HELIOS);
    chaos_engine->RegisterPattern(ChaosEngine::RIPPLE);
    //chaos_engine->RegisterPattern(ChaosEngine::DOUBLEMARQEE);
    chaos_engine->RegisterPattern(ChaosEngine::CONFETTI);
    chaos_engine->RegisterPattern(ChaosEngine::PIXELS);
    chaos_engine->RegisterPattern(ChaosEngine::PACIFICA); 
    chaos_engine->RegisterPattern(ChaosEngine::NOISE);
    
    chaos_engine->Start();
}

/* main loop */
void loop()
{
    // generate the current pattern
    chaos_engine->GetRunningPattern()->Generate(leds_A); 
    chaos_engine->Update(leds_A);

    uint8_t index_offset_b = random(10, 25);
    uint8_t index_offset_c = random(30, 45);
    
    for (int i = 0; i < NUM_LEDS; i++) {
        leds_B[i] = leds_A[(i + index_offset_b) % NUM_LEDS];
        leds_C[i] = leds_A[(i + index_offset_c) % NUM_LEDS];
    }
    
    FastLED.show();
    FastLED.delay(chaos_engine->GetTotalDelay());
}
