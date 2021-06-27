 #include <FastLED.h>
 
#include "config.h"
#include "chaos_engine.h"

#include "pattern.h"

ChaosEngine* chaos_engine;

// The leds
CRGB leds_A[NUM_LEDS];
CRGB leds_B[NUM_LEDS];
CRGB leds_C[NUM_LEDS];
int strip_offsets[NUM_LED_STRIPS];
uint8_t index_offset_b = 0; //random(10, 25);
uint8_t index_offset_c = 0; //random(30, 45);

uint8_t PatternSet[] = {ChaosEngine::RIPPLE, ChaosEngine::PIXELS, ChaosEngine::DOUBLEMARQEE, ChaosEngine::CONFETTI, ChaosEngine::NOISE};
//uint8_t PatternSet[] = {ChaosEngine::HELIOS, ChaosEngine::RIPPLE, ChaosEngine::PIXELS, ChaosEngine::DOUBLEMARQEE, ChaosEngine::CONFETTI, ChaosEngine::NOISE, ChaosEngine::PACIFICA};

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

    for (int i = 0; i < ARRAY_SIZE(PatternSet); i++) {
        chaos_engine->RegisterPattern(PatternSet[i]);
    }
    
    chaos_engine->Start();
}

/* main loop */
void loop()
{
    // generate the current pattern
    chaos_engine->GetRunningPattern()->Generate(leds_A); 
    chaos_engine->Update(leds_A);

    for (int i = 0; i < NUM_LEDS; i++) {
        leds_B[i] = leds_A[i];
    }
    
    
    FastLED.show();
    FastLED.delay(chaos_engine->GetTotalDelay());
}
