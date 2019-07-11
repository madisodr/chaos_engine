#ifndef RIPPLE_H
#define RIPPLE_H

#include <FastLED.h>
#include "config.h"
#include "utils.h"

#define DELAY 20
class Ripple : public Pattern
{
    public:
        Ripple(uint16_t _time);
        ~Ripple();

        void Run();
    private:
    int m_color;
    int m_center;
    int m_step;
    const int m_max_steps = 15;
    const float m_fade_rate = .9;
    int m_diff;

    inline int Wrap(int step);
};

Ripple::Ripple(uint16_t _time) : Pattern(_time) {
  m_center = 0;
m_step = -1;
    m_max_brightness = MAX_BRIGHTNESS;
}

Ripple::~Ripple() {

}

inline int Ripple::Wrap(int step) {
  if (step < 0)
    return REAL_NUM_LEDS + step;
  else if (step > REAL_NUM_LEDS - 1)
    return step - REAL_NUM_LEDS;
  else
    return step;
}

void Ripple::Run() {
  fadeToBlackBy(leds, REAL_NUM_LEDS, 5);

  if (m_step == -1) {
    m_center = random(REAL_NUM_LEDS);

    if (m_center > LEFT_MID && m_center <= RIGHT_MID) {
      m_color = 20 + random8(36);
    } else {
      m_color = 120 + random8(36);;
    }
    m_step = 0;
  }
  if (m_step == 0) {
    leds[m_center] = CHSV(m_color, 255, MAX_BRIGHTNESS);
    m_step ++;
  } else {
    if (m_step < m_max_steps) {
      leds[Wrap(m_center + m_step)] = CHSV(m_color, 255, pow(m_fade_rate, m_step) * MAX_BRIGHTNESS);
      leds[Wrap(m_center - m_step)] = CHSV(m_color, 255, pow(m_fade_rate, m_step) * MAX_BRIGHTNESS);
      if (m_step > 3) {
        leds[Wrap(m_center + m_step - 3)] = CHSV(m_color, 255, pow(m_fade_rate, m_step - 2) * MAX_BRIGHTNESS);
        leds[Wrap(m_center - m_step + 3)] = CHSV(m_color, 255, pow(m_fade_rate, m_step - 2) * MAX_BRIGHTNESS);
      }
      m_step++;
    } else {
      m_step = -1;
    }
  }

  delay(DELAY);
}


#endif // RIPPLE_H
