#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "pattern.h"
#include "utils.h"

class Playlist
{
    public:
        Playlist(Pattern** _list, uint8_t _pattern_count);
        ~Playlist();

        Pattern* GetCurrent() const;
        Pattern* GetNext() const;

        void SetupNextPattern(bool random_pat);
        Pattern* GetPattern(uint8_t index) const;
        void SetCurrentPattern(Pattern* p);
        uint8_t GetIndex(Pattern* p) const;
        uint8_t GetTotalDelay() const;
        void SetTotalDelay(uint8_t total_delay);

    private:
        Pattern** m_list;
        Pattern* m_current;
        Pattern* m_next;
        uint8_t m_pattern_count;
        uint16_t m_total_delay;
};

Playlist::Playlist(Pattern** _list, uint8_t _pattern_count)
{
    m_list = _list;
    m_pattern_count = _pattern_count;
    m_current = m_list[0];
    m_next = m_list[1];
}

Playlist::~Playlist()
{
    for (int i = 0; i < m_pattern_count; i++) {
        delete m_list[i];
    }

    delete[] m_list;
}

// returns currently playing pattern
inline Pattern* Playlist::GetCurrent() const
{
    return m_current;
}

inline Pattern* Playlist::GetNext() const
{
    return m_next;
}

uint8_t Playlist::GetIndex(Pattern* p) const
{
    for (int i = 0; i < m_pattern_count; i++) {
        if (p == m_list[i]) {
            return i;
        }
    }
}

void Playlist::SetupNextPattern(bool random_pat = false)
{
    uint8_t idx = GetIndex(m_next);
    uint8_t curr = GetIndex(m_next);

    if (random_pat) {
        do {
            idx = random8(m_pattern_count);
        } while (idx == curr);
    } else {
        idx = modulo(idx + 1, m_pattern_count);
    }

    m_next = m_list[idx];
}

inline Pattern* Playlist::GetPattern(uint8_t index) const
{
    return m_list[index];
}

inline void Playlist::SetCurrentPattern(Pattern* p)
{
    m_current = p;
}

inline uint8_t Playlist::GetTotalDelay() const
{
    return m_total_delay;
}

inline void Playlist::SetTotalDelay(uint8_t total_delay)
{
    m_total_delay = total_delay;
}

#endif // PLAYLIST_H
