#include <Vector.h>
#include <FastLED.h>

#include "pattern.h"
#include "confetti.h"
#include "noise.h"
#include "ripple.h"
#include "moving_pixels.h"
#include "double_marqee.h"
#include "helios.h"
#include "pacifica.h"

const int MAX_PATTERN_COUNT = 10;

class LightStrip
{
    public:
        LightStrip(CRGB* _leds, uint8_t _pin) : m_leds(_leds), m_pin(_pin)
        {
            
        }
        ~LightStrip();

    private:
        CRGB* m_leds;
        uint8_t m_pin;
};

class ChaosEngine
{
  public:
    ChaosEngine();
    ~ChaosEngine();

    void Start();

    void Update(CRGB* current_pattern);
    void UpdateDelay();
    uint16_t GetTotalDelay();

    enum PatternIDs { HELIOS, RIPPLE, PACIFICA, CONFETTI, PIXELS, NOISE, DOUBLEMARQEE };
    
    void RegisterPattern(uint8_t i);
    Pattern* GetRunningPattern();
    void SetupNextPattern();

    void Glitch(CRGB* leds);
    void ResetGlitch();
  private:
    uint8_t m_total_delay;
    
    uint16_t m_start_blending_timer; // when to start blending to the next pattern
    bool m_is_blending;
    float m_blend_amount;
    const float m_blend_step = 0.5;
    
    Pattern* pattern_storage[MAX_PATTERN_COUNT];
    Vector<Pattern*> m_patterns;
    Pattern* m_curr_pattern;
    Pattern* m_next_pattern;
 
    CRGB m_blending_pattern[NUM_LEDS];
    
    bool m_glitching_enabled;
    bool m_is_glitching;
    uint32_t m_next_glitch;
    uint8_t m_glitch_frames;
    // uint8_t m_glitchStrip;
    uint32_t m_frame;
};

ChaosEngine::ChaosEngine() 
{
    m_is_blending = false;
    m_blend_amount = 0.0;
    m_patterns.setStorage(pattern_storage);
     m_glitching_enabled = false;
     
    if (CONFIG_GLITCHING_ENABLED) {
        m_glitching_enabled = true;        
        ResetGlitch();
    }
}

void ChaosEngine::Start()
{   
    // m_curr_pattern = m_patterns[random(m_patterns.size())]; // Randomly pick the first pattern
    m_curr_pattern = m_patterns[0]; // Use the first pattern in our list. useful for debug purposes 
    
    m_total_delay = m_curr_pattern->GetDelay();
    SetupNextPattern();
    Pattern::SetGlobalHue(random(255));
    m_start_blending_timer = millis() + (m_curr_pattern->GetTime() * BLEND_TIME_MULTIPLIER);
}

void ChaosEngine::Update(CRGB* leds)
{
    EVERY_N_MILLISECONDS(500) {
        Pattern::UpdateGlobalHue();
    }

    if (millis() >= m_start_blending_timer) {
        m_is_blending = true;
    }

    if (m_is_blending && m_next_pattern != NULL) {
        // generate and blend the next pattern into the original
        m_next_pattern->Generate(m_blending_pattern);
        
        blend(leds, m_blending_pattern, leds, NUM_LEDS, m_blend_amount);

        // Adjust the blend amount of the second pattern into the first.
        EVERY_N_MILLISECONDS(int((m_curr_pattern->GetTime() * BLEND_TIME_MULTIPLIER) / 255)) {
            if (m_blend_amount < 255) {
                m_blend_amount += m_blend_step;
            }
        }

        // delay drift so the delay between patterns changes over smoothly
        EVERY_N_MILLISECONDS(DELAY_DRIFT) {
            UpdateDelay();
        }
    }

    EVERY_N_SECONDS_I(timer, m_curr_pattern->GetTime()) {
        // reset blending for new pattern
        m_start_blending_timer = millis() + (m_curr_pattern->GetTime() * BLEND_TIME_MULTIPLIER);
        m_blend_amount = 0;
        m_is_blending = false;

        m_curr_pattern = m_next_pattern;
        SetupNextPattern();

        // Update timer period to new pattern's length
        timer.setPeriod(m_curr_pattern->GetTime());
    }

    if (m_glitching_enabled) {
        Glitch(leds);
        m_frame++;
    }
}

uint16_t ChaosEngine::GetTotalDelay()
{
    return m_total_delay;
}

void ChaosEngine::UpdateDelay()
{
    if (m_total_delay != m_next_pattern->GetDelay()) {
        m_total_delay < m_next_pattern->GetDelay() ? m_total_delay++ : m_total_delay--;
    }
}

void ChaosEngine::RegisterPattern(const uint8_t pattern_id)
{
    if (m_patterns.size() > m_patterns.max_size()) {
        return;
    }
    
    Pattern* p = NULL;

    switch (pattern_id) {
        case HELIOS:
            p = new Helios(40);
            break;
        case RIPPLE:
            p = new Ripple(70);
            break;
        case PACIFICA:
            p = new Pacifica(50);
            break;
        case CONFETTI:
            p = new Confetti(60);
            break;
        case PIXELS:
            p = new MovingPixels(70);
            break;
        case NOISE:
            p = new Noise(50);
            break;
        case DOUBLEMARQEE:
            p = new DoubleMarqee(50);
            break;
    }

    if (p != NULL) {
        m_patterns.push_back(p);
    }
}

void ChaosEngine::SetupNextPattern()
{
    do { 
        m_next_pattern = m_patterns[random(m_patterns.size())];
    } while (m_next_pattern == m_curr_pattern);
}

Pattern* ChaosEngine::GetRunningPattern()
{
    return m_curr_pattern;
}

void ChaosEngine::ResetGlitch()
{
    m_is_glitching = false;
    m_next_glitch = random(60);
    m_glitch_frames = random8(30);
    m_frame = 0;
}

// Random strip flicker
void ChaosEngine::Glitch(CRGB* leds)
{
    if (m_frame >= m_next_glitch) {
        m_is_glitching = true;
    }

    if (m_is_glitching) {
        int shift = random(16);
        if (m_frame >= m_next_glitch && m_frame < m_next_glitch + m_glitch_frames) {
            int start = random(NUM_LEDS); // * m_glitchStrip;
            int led_count = random(NUM_LEDS / 2);
            for (int i = start; i < start + led_count; ++i) {
                if (random(100) < 40) {
                    leds[i % NUM_LEDS] = Pattern::GetGlobalCRGB(shift);
                }
            }
        } else if (m_frame >= m_next_glitch + m_glitch_frames) {
            ResetGlitch();
        }
    }
}
