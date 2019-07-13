#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "pattern.h"
#include "utils.h"

class Playlist
{
    public:
        Playlist(Pattern** _list, uint8_t _pattern_count);
        ~Playlist();

        uint8_t Current();
        uint8_t GetNextPattern(bool random_pat);
        Pattern* GetPattern(int index);
        inline void Restart();
        void Seek(bool next);
        inline uint8_t GetIndex();

    private:
        Pattern** m_list;
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
}

Playlist::~Playlist() {
    for (int i = 0; i < m_pattern_count; i++) {
        delete m_list[i];
    }

    delete m_list;
}

// returns currently playing pattern
uint8_t Playlist::Current() {
    return m_index;
}

uint8_t Playlist::GetNextPattern(bool random_pat = false) {
    if (random_pat) {
        int x = m_index;
        while (x == m_index) {
            x = random(m_pattern_count - 1);
        }

        return x;
    } else {
        return modulo(m_index++, m_pattern_count);
    }
}

Pattern* Playlist::GetPattern(int index) {
    if (index < 0 || index > m_pattern_count) {
        return NULL;
    } else {
        return m_list[index];
    }
}

void Playlist::Restart() {
    m_index = 0;
}

uint8_t Playlist::GetIndex() {
    return m_index;
}

#endif // PLAYLIST_H
