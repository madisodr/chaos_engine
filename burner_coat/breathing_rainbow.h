#ifndef BREATHING_RAINBOW_H
#define BREATHING_RAINBOW_H

#include <FastLED.h>
#include "config.h"
#include "pattern.h"
#include "utils.h"

class BreathingRainbow : public Pattern
{
    public:
        BreathingRainbow(uint16_t _time);
        ~BreathingRainbow();

        void Run();
    private:
        const int m_breath_speed = 5000.0 * PI;
        const byte m_delta = 255 / REAL_NUM_LEDS;
        uint8_t m_hue;
};

BreathingRainbow::BreathingRainbow(uint16_t _time) : Pattern(_time) {
    m_max_brightness = MAX_BRIGHTNESS;
}

BreathingRainbow::~BreathingRainbow() {

}

void BreathingRainbow::Run() {
    float breath = (sin(millis() / m_breath_speed)) * 108.0;
    breath = map(breath, 0, min(MAX_BRIGHTNESS, m_max_brightness), 5, 40);
    FastLED.setBrightness(breath);

    // increase the hue by 1 each time
    m_hue++;

    int led_growth = 1;
    EVERY_N_MILLIS(50) {
        m_num_leds += led_growth;
    }

    m_num_leds = min(REAL_NUM_LEDS, m_num_leds);

    fill_rainbow(leds, m_num_leds, m_hue, m_delta);
    delay(20);
}

#endif // BREATHING_RAINBOW_H
