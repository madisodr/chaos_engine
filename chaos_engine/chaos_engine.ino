 #include <FastLED.h>
 
#include "config.h"
#include "chaos_engine.h"

#include "pattern.h"

ChaosEngine* chaos_engine;

// The leds
CRGB leds[NUM_LEDS];

/* setup */
void setup()
{
    LEDS.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    randomSeed(millis() + analogRead(0));
    
    // Set the maximum power the LEDs can pull
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_VOLTS);
    chaos_engine = new ChaosEngine();
 
    chaos_engine->RegisterPattern(ChaosEngine::HELIOS);
    chaos_engine->RegisterPattern(ChaosEngine::RIPPLE);
    chaos_engine->RegisterPattern(ChaosEngine::DOUBLEMARQEE);
    chaos_engine->RegisterPattern(ChaosEngine::CONFETTI);
    chaos_engine->RegisterPattern(ChaosEngine::PIXELS);
    chaos_engine->RegisterPattern(ChaosEngine::NOISE);
    
    chaos_engine->Start();
}

/* main loop */
void loop()
{
    // generate the current pattern
    chaos_engine->GetRunningPattern()->Generate(leds); 
    chaos_engine->Update(leds);
    //Pattern::Glitch(leds);
    FastLED.show();
    FastLED.delay(chaos_engine->GetTotalDelay());
}
