#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "pattern.h"
#include "utils.h"

class Playlist
{
    public:
        Playlist(Pattern** _list, uint8_t _pattern_count);
        ~Playlist();

        Pattern* Current();
        Pattern* GetNextPattern();
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
Pattern* Playlist::Current() {
    return m_list[m_index];
}

Pattern* Playlist::GetNextPattern() {
    if (m_index+1 >= m_pattern_count) {
        return m_list[0];
    } else {
        return m_list[m_index + 1];
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

void Playlist::Seek(bool next = true) {
    if (next)
        m_index++;
    else
        m_index--;

    m_index = modulo(m_index, m_pattern_count);
    m_paused = false;
}

uint8_t Playlist::GetIndex() {
    return m_index;
}

#endif // PLAYLIST_H
