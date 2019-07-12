#ifndef DOUBLE_MARQEE_H
#define DOUBLE_MARQEE_H

#include <FastLED.h>
#include "config.h"
#include "utils.h"

class DoubleMarqee : public Pattern
{
  public:
    DoubleMarqee(uint16_t _time, bool _rainbow);
    ~DoubleMarqee();

    void Run();
  private:
    const int marqee_length = 12;
    bool m_rainbow;

};

DoubleMarqee::DoubleMarqee(uint16_t _time, bool _rainbow): Pattern(_time) {
  m_rainbow = _rainbow;
}

DoubleMarqee::~DoubleMarqee() {

}

void DoubleMarqee::Run() {
  CRGB color;
  int l_idx, r_idx;
  if (m_rainbow) {
    for (uint16_t j = 255; j > 0; j--) { // cycle of all colors on wheel
      for (uint16_t i = 0; i < STRIP_LENGTH; i++) {
        color = wheel(((i * 256 / (STRIP_LENGTH)) + j) & 255);
        l_idx = modulo(LEFT_TOP - i, REAL_NUM_LEDS);
        leds[l_idx] = color;

        r_idx = modulo(RIGHT_TOP + i, REAL_NUM_LEDS);
        leds[r_idx] = color;
      }

      FastLED.setBrightness(MAX_BRIGHTNESS);
      FastLED.show();
      delay(10);
    }
  } else {
    color = P_ORG;
    for (uint16_t q = 0; q < marqee_length; q++) {
      for (uint16_t i = 0; i < STRIP_LENGTH; i = i + marqee_length) {
        for (uint16_t z = 0; z < marqee_length; z++ ) {
          l_idx = modulo(LEFT_TOP - i - q + z, REAL_NUM_LEDS);
          leds[l_idx] = color;
          leds[l_idx].maximizeBrightness(10 + (z * 40));

          r_idx = modulo(RIGHT_TOP + i + q - z, REAL_NUM_LEDS);
          leds[r_idx] = color;
          leds[r_idx].maximizeBrightness(10 + (z * 40));
        }
      }

      FastLED.show();
      delay(70);
    }
  }
}

#endif // DOUBLE_MARQEE_H
