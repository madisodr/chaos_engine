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

class ChaosEngine
{
  public:
    ChaosEngine();
    ~ChaosEngine();

    void Update(CRGB* current_pattern);

    void UpdateDelay();
    uint16_t GetTotalDelay();

    
    void Start();
    
    void RegisterPattern(uint8_t i);
    Pattern* GetRunningPattern();
    void SetupNextPattern();

    enum PatternIDs { HELIOS, RIPPLE, PACIFICA, CONFETTI, PIXELS, NOISE, DOUBLEMARQEE };
    
  private:
    uint8_t m_total_delay;
    
    uint16_t m_start_blending_timer; // when to start blending to the next pattern
    bool m_is_blending;
    float m_blend_amount;
    
    Pattern* pattern_storage[MAX_PATTERN_COUNT];
    Vector<Pattern*> m_patterns;
    Pattern* m_curr_pattern;
    Pattern* m_next_pattern;
 
    CRGB m_blending_pattern[NUM_LEDS];
    
};

ChaosEngine::ChaosEngine() 
{
    m_is_blending = false;
    m_blend_amount = 0.0;
    m_patterns.setStorage(pattern_storage);
}

void ChaosEngine::Update(CRGB* current_pattern)
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
        blend(current_pattern, m_blending_pattern, current_pattern, NUM_LEDS, m_blend_amount);

        // Adjust the blend amount of the second pattern into the first.
        EVERY_N_MILLISECONDS(int((m_curr_pattern->GetTime() * BLEND_TIME_MULTIPLIER) / 255)) {
            if (m_blend_amount < 255) {
                m_blend_amount += 1.0;
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
}

uint16_t ChaosEngine::GetTotalDelay()
{
    return m_total_delay;
}

void ChaosEngine::UpdateDelay()
{
    if (m_total_delay != m_next_pattern->GetDelay()) {
        if (m_total_delay < m_next_pattern->GetDelay()) {
            m_total_delay++;
        } else {
            m_total_delay--;
        }
    }
}

void ChaosEngine::RegisterPattern(uint8_t pattern_id)
{
    if (m_patterns.size() > m_patterns.max_size()) {
        return;
    }
    
    Pattern* p = NULL;

    switch (pattern_id) {
        case HELIOS:
            p = new Helios(PATTERN_LENGTH,30);
            break;
        case RIPPLE:
            p = new Ripple(PATTERN_LENGTH,70);
            break;
        case PACIFICA:
            p = new Pacifica(PATTERN_LENGTH, 50);
            break;
        case CONFETTI:
            p = new Confetti(PATTERN_LENGTH,60);
            break;
        case PIXELS:
            p = new MovingPixels(PATTERN_LENGTH, 70);
            break;
        case NOISE:
            p = new Noise(PATTERN_LENGTH, 50);
            break;
        case DOUBLEMARQEE:
            p = new DoubleMarqee(PATTERN_LENGTH, 50);
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

void ChaosEngine::Start()
{   
    m_curr_pattern = m_patterns[random(m_patterns.size())];
    m_total_delay = m_curr_pattern->GetDelay();
    SetupNextPattern();
    Pattern::SetGlobalHue(random());
    m_start_blending_timer = millis() + (m_curr_pattern->GetTime() * BLEND_TIME_MULTIPLIER);
}
