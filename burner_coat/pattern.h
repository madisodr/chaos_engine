#ifndef PATTERN_H
#define PATTERN_H

class Pattern
{
    public:
        Pattern(uint16_t _time);
        ~Pattern();

        virtual void Run();
        void Reset();

        void Enable();
        void Disable();
        uint16_t m_time;
        uint8_t m_max_brightness;
        int m_num_leds;
    private:


};

Pattern::Pattern(uint16_t _time): m_time(_time) {
    m_num_leds = 1;
}

Pattern::~Pattern() {
}

void Pattern::Reset() {
    int m_num_leds = 1;
}


void Pattern::Enable() {
    m_max_brightness = MAX_BRIGHTNESS;
}

void Pattern::Disable() {
    m_max_brightness = 0;
}

#endif // PATTERN_H
