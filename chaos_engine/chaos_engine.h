#ifndef CHAOS_ENGINE_H
#define CHAOS_ENGINE_H


#include <FastLED.h>
#include "config.h"
#include "queue.h"

// return a random double between 0.0 and 1.0
float RandomFloat()
{
    return (float) random(101) / (float) 100.0;
}

class Spark
{
    public:
        CRGB m_color;
        uint16_t m_birth_time;
        uint16_t m_last_update;
        float m_velocity;
        uint8_t m_position;

        Spark(CRGB starColor, uint8_t pos, double maxSpeed)
        {
            m_position = pos;
            m_velocity = RandomFloat() * maxSpeed * 2 - maxSpeed;
            m_color = starColor;
            m_birth_time = millis();
            m_last_update = millis();
        }

        double GetAge()
        {
            return (millis() - m_birth_time);
        }

        // As the particle ages we actively fade its color and slow its speed
        void Update()
        {
            float delta = (millis() - m_last_update);
            m_position += m_velocity * delta;
            m_last_update = millis();

            m_velocity -= (2 * m_velocity * delta);

            m_color = m_color.fadeToBlackBy((float)RandomFloat() * 0.1f);
        }
};

class ChaosEngine : public Pattern
{
    public:
        ChaosEngine(uint16_t _time, uint16_t _delay);
        ~ChaosEngine();

        void Generate(CRGB* arr);
        void Reset();


        // SOME OF THESE MIGHT SHOULD BE CONSTANTS
        bool       m_blend;
        double     m_max_speed;                // Max velocity
        double     m_new_particle_probability; // Odds of new particle
        double     m_particle_preignition_time; // How long to "wink"
        double     m_particle_ignition;        // How long to "flash"
        double     m_particle_hold_time;       // Main lifecycle time
        double     m_particle_fade_time;       // Fade out time
        double     m_particle_size;            // Size of the particle

    private:
        Queue<Spark*> m_sparks;
};

#define MAX_SPARKS 16

ChaosEngine::ChaosEngine(uint16_t _time, uint16_t _delay) : Pattern(_time, _delay)
{
    m_blend                     = true;
    m_max_speed                 = 100.0f;
    m_new_particle_probability  = 0.01;
    m_particle_preignition_time = 0.0;
    m_particle_ignition         = 0.2;
    m_particle_hold_time        = 0.0;
    m_particle_fade_time        = 2.0;
    m_particle_size             = 0.0;
    m_sparks = Queue<Spark*>(MAX_SPARKS);
}

ChaosEngine::~ChaosEngine() {}

void ChaosEngine::Reset()
{
}

void ChaosEngine::Generate(CRGB* leds)
{
    for (int iPass = 0; iPass < NUM_LEDS / 50; iPass++)
    {
        if (RandomFloat() < m_new_particle_probability)
        {
            // Pick a random color and location.
            // If you don't have FastLED palettes, all you need to do
            // here is generate a random color.

            uint8_t start_pos = random8(NUM_LEDS);
            CRGB color = wheel(Pattern::GetGlobalHue());

            int c = random(10, 50);
            double multiplier = RandomFloat() * 3;

            for (int i = 1; i < c; i++)
            {
                Spark* spark = new Spark(color, start_pos, m_max_speed * RandomFloat() * multiplier);
                m_sparks.Push(spark);
            }
        }
    }

    FastLED.clear();
    for (int i = 0; i < m_sparks.Count(); i++)
    {
        Spark* star = m_sparks.At(i);
        star->Update();

        CRGB c = star->m_color;

        // If the star is brand new, it flashes white briefly.
        // Otherwise it just fades over time.

        double fade = 0.0f;
        if (star->GetAge() > m_particle_preignition_time && star->GetAge() < m_particle_ignition + m_particle_preignition_time)
        {
            c = CRGB::White;
        }
        else
        {
            // Figure out how much to fade and shrink the star based on
            // its age relative to its lifetime

            double age = star->GetAge();
            if (age < m_particle_preignition_time)
                fade = 1.0 - (age / m_particle_preignition_time);
            else
            {
                age -= m_particle_preignition_time;

                if (age < m_particle_hold_time + m_particle_ignition)
                    fade = 0.0f;                  // Just born
                else if (age > m_particle_hold_time + m_particle_ignition + m_particle_fade_time)
                    fade = 1.0f;                  // Black hole, all faded out
                else
                {
                    age -= (m_particle_hold_time + m_particle_ignition);
                    fade = (age / m_particle_fade_time);  // Fading star
                }
            }
            c = c.fadeToBlackBy((float)fade);
        }
        m_particle_size = (1 - fade) * 5;

        // Because I support antialiasing and partial pixels, this takes a
        // non-integer number of pixels to draw.  But if you just made it
        // plot 'm_particle_size' pixels in int form, you'd be 99% of the way there
        leds[star->m_position] = c;
        //graphics.DrawPixels(star._position, (uint)m_particle_size, c);

    }

    // Remove any particles who have completed their lifespan

    while (m_sparks.Count() > 0 && m_sparks.Peek()->GetAge() >
            m_particle_hold_time +  m_particle_ignition + m_particle_fade_time)
    {
        m_sparks.Pop();
    }


    leds[0] = wheel(GetGlobalHue());
}

#endif // CHAOS_ENGINE_H
