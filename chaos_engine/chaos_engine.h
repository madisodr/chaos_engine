#include <vector>

#include "pattern.h"

class ChaosEngine
{
  public:
    ChaosEngine();
    ~ChaosEngine();

    void Update();
    
    void SetGlobalHue(uint8_t _hue);
    uint8_t GetGlobalHue();
    
    void Start();
    void Stop();
    
    void RegisterPattern(Pattern* p);
    
  private:
    uint8_t global_hue;
    QueueList<Pattern*> playlist;
    std::vector<Pattern*> patterns;
};

void ChaosEngine::Update()
{
    this->UpdateGlobalHue();
}

void ChaosEngine::SetGlobalHue(uint8_t _hue)
{   
    this->global_hue = _hue;
}

uint8_t ChaosEngine::GetGlobalHue()
{
    return this->global_hue;
}

void ChaosEngine::UpdateGlobalHue()
{
    global_hue++;
    global_hue %= 255;
}

void ChaosEngine::RegisterPattern(Pattern* p)
{
    if (p == NULL) {
        return;   
    }

    this->patterns.push_back(p);
}


void ChaosEngine::Start() {}
void ChaosEngine::Stop() {}
