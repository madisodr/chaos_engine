#ifndef CHAOS_ENGINE_H
#define CHAOS_ENGINE_H

#include <cppQueue.h>
#include <FastLED.h>
#include "config.h"

class ChaosEngine : public Pattern
{
    public:
        // Each particle in the particle system remembers its color,
        // birth time, postion, velocity, etc.  If you are not using DateTime,
        // all you need in its place is a fractional number of seconds elapsed, which is
        // all I use it for.  So timer()/1000.0 or whatever should suffice as well.

        class Particle
        {
            public:
                CRGB m_star_color;
                uint16_t m_birth_time;
                uint16_t m_last_update;
                double m_velocity;
                double m_position;

                //static Random _rand = new Random();
                Particle() {}

                Particle(CRGB _star_color, double _pos, double _max_speed)
                {
                    m_position = _pos;
                    m_velocity = ChaosEngine::RandomDouble() * _max_speed * 2 - _max_speed;
                    m_star_color = _star_color;
                    m_birth_time = millis() / 1000;
                    m_last_update = millis() / 1000;
                }

                double GetAge()
                {
                    return (millis() / 1000) - m_birth_time;
                }

                // As the particle ages we actively fade its color and slow its speed

                void Update()
                {
                    double now = millis() / 1000;
                    m_last_update = now;

                    double delta_time = now - m_last_update;
                    m_position += m_velocity * delta_time;

                    m_velocity -= (2 * m_velocity * delta_time);

                    m_star_color = m_star_color.fadeToBlackBy(ChaosEngine::RandomDouble() * 0.1f);
                }
        };

        ChaosEngine(uint16_t _time, uint16_t _delay);
        ~ChaosEngine();

        void Generate(CRGB* arr);
        void Reset();
        static float RandomDouble();
    protected:

        bool Blend;
        double MaxSpeed;        // Max velocity
        double NewParticleProbability;         // Odds of new particle
        double ParticlePreignitonTime;          // How long to "wink"
        double ParticleIgnition;          // How long to "flash"
        double ParticleHoldTime;         // Main lifecycle time
        double ParticleFadeTime;          // Fade out time
        double ParticleSize;         // Size of the particle

        Queue* _Particles;

    private:
};

ChaosEngine::ChaosEngine(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    Blend = true;
    MaxSpeed = 375.0f;        // Max velocity
    NewParticleProbability = 0.01f;         // Odds of new particle
    ParticlePreignitonTime = 0.0f;          // How long to "wink"
    ParticleIgnition = 0.2f;          // How long to "flash"
    ParticleHoldTime = 0.00f;         // Main lifecycle time
    ParticleFadeTime = 2.0f;          // Fade out time
    ParticleSize = 0.00f;         // Size of the particle

    _Particles = new Queue(sizeof(Particle*));
}

ChaosEngine::~ChaosEngine() {}

// return a random double between 0.0 and 1.0
float ChaosEngine::RandomDouble()
{
    return (double) random(101) / (double) 100.0;
}

void ChaosEngine::Generate(CRGB* arr)
{

    fadeToBlackBy(arr, NUM_LEDS, 30);

    // Randomly create some new stars this frame; the number we create is tied
    // to the size of the display so that the display size can change and
    // the "effect density" will stay the same
    for (int i = 0; i < NUM_LEDS / 50; i++) {
        if (RandomDouble() < NewParticleProbability) {
            // Pick a random color and location.
            // If you don't have FastLED palettes, all you need to do
            // here is generate a random color.

            uint32_t iStartPos = random(NUM_LEDS);
            CRGB color = LED_COLOR_LOW;
            int c = random(10, 50);
            double multiplier = RandomDouble() * 3;

            for (int i = 1; i < c; i++) {
                Particle* particle = new Particle(color, iStartPos, MaxSpeed * RandomDouble() * multiplier);
                _Particles->push(particle);
            }
        }
    }

    // In the degenerate case of particles not aging out for some reason,
    // we need to set a pseudo-realistic upper bound, and the very number of
    // possible pixels seems like a reasonable one

    while (_Particles->getCount() > NUM_LEDS) {
        _Particles->pop();
    }

    for (int idx = 0; idx < _Particles->getCount(); idx++) {
        Particle* star = (Particle*) _Particles->peek();

        star->Update();

        CRGB c = star->m_star_color;

        // If the star is brand new, it flashes white briefly.
        // Otherwise it just fades over time.

        double fade = 0.0f;
        if (star->GetAge() > ParticlePreignitonTime && star->GetAge() < ParticleIgnition + ParticlePreignitonTime) {
            c = CRGB::White;
        } else {
            // Figure out how much to fade and shrink the star based on
            // its age relative to its lifetime

            double age = star->GetAge();
            if (age < ParticlePreignitonTime) {
                fade = 1.0 - (age / ParticlePreignitonTime);
            } else {
                age -= ParticlePreignitonTime;

                if (age < ParticleHoldTime + ParticleIgnition) {
                    fade = 0.0f;                  // Just born
                } else if (age > ParticleHoldTime + ParticleIgnition + ParticleFadeTime) {
                    fade = 1.0f;                  // Black hole, all faded out
                } else {
                    age -= (ParticleHoldTime + ParticleIgnition);
                    fade = (age / ParticleFadeTime);  // Fading star
                }
            }
            c.fadeToBlackBy(fade);
        }

        ParticleSize = (1 - fade) * 5;

        // Because I support antialiasing and partial pixels, this takes a
        // non-integer number of pixels to draw.  But if you just made it
        // plot 'ParticleSize' pixels in int form, you'd be 99% of the way there

        int left = max(0, star->m_position - ParticleSize / 2);
        int right = min(NUM_LEDS - 1, star->m_position + ParticleSize / 2);

        //for (int i = left; i < right; i++) {
        arr[(uint32_t)star->m_position] = c;

    }

    // Remove any particles who have completed their lifespan
    Particle* p = (Particle*)_Particles->peek();
    while (_Particles->getCount() > 0 && p->GetAge() > ParticleHoldTime +  ParticleIgnition + ParticleFadeTime) {
        _Particles->pop();
    }

    arr[0] = LED_COLOR_HIGH;
    arr[NUM_LEDS - 1] = LED_COLOR_HIGH;
}

void ChaosEngine::Reset()
{
}


#endif // CHAOS_ENGINE_H
