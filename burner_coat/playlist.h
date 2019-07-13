#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "pattern.h"
#include "utils.h"

class Playlist
{
    public:
        Playlist(Pattern** _list, uint8_t _pattern_count);
        ~Playlist();

        Pattern* GetCurrent();
        Pattern* GetNext();
        
        void SetupNextPattern(bool random_pat);
        Pattern* GetPattern(uint8_t index);
        inline void Restart();
        inline void SetCurrentPattern(Pattern* p);
        inline uint8_t GetIndex();

    private:
        Pattern** m_list;
        Pattern* m_current;
        Pattern* m_next;
        uint8_t m_index;
        bool m_paused;
        uint8_t m_pattern_count;
};

Playlist::Playlist(Pattern** _list, uint8_t _pattern_count)
{
    m_list = _list;
    m_index = 0;
    m_paused = false;
    m_pattern_count = _pattern_count;
    m_current = m_list[0];
    m_next = m_list[1];
}

Playlist::~Playlist()
{
    for (int i = 0; i < m_pattern_count; i++) {
        delete m_list[i];
    }

    delete m_list;
}

// returns currently playing pattern
Pattern* Playlist::GetCurrent()
{
    return m_current;
}

Pattern* Playlist::GetNext()
{
    return m_next;
}

void Playlist::SetupNextPattern(bool random_pat = false)
{
    if (random_pat) {
        int x = m_index;
        while (x == m_index) {
            m_index = random(m_pattern_count);
        }
    } else {
        m_index = modulo(m_index+1, m_pattern_count);
    }
    
    m_next = m_list[m_index];
}

Pattern* Playlist::GetPattern(uint8_t index)
{
    if (index > m_pattern_count) {
        return NULL;
    } else {
        return m_list[index];
    }
}

void Playlist::SetCurrentPattern(Pattern* p)
{
    m_current = p;
}

void Playlist::Restart() {
    m_index = 0;
    m_current = m_list[0];
}

#endif // PLAYLIST_H
