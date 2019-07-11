#ifndef PATTERN_H
#define PATTERN_H
class Pattern
{
    public:
        Pattern(uint16_t _time): m_time(_time) {};
        ~Pattern();

        virtual void Run();
        void Enable();
        void Disable();
        uint16_t m_time;
        uint8_t m_max_brightness;
        private:
        
        
};

Pattern::~Pattern() {
  // Empty Constructor
}


void Pattern::Enable() {
    m_max_brightness = MAX_BRIGHTNESS;
}

void Pattern::Disable() {
    m_max_brightness = 0;
}

#endif // PATTERN_H
