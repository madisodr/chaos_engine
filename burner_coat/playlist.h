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
        void SetPattern(Pattern* br);
        void Restart();
        void Seek(bool next);
        void Pause();
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

void Playlist::SetPattern(Pattern* br) {
  m_list[0] = br;
}

// returns currently playing pattern
Pattern* Playlist::Current() {
    if(m_index < 0 || m_index > m_pattern_count)
      return m_list[0];
    else 
      return m_list[m_index];
}

void Playlist::Restart() {
    m_index = 0;
}

void Playlist::Seek(bool next = true) {
    if(next)
        m_index++;
    else
        m_index--;

    m_index = modulo(m_index, m_pattern_count);

    m_paused = false;
}

void Playlist::Pause() {
    m_paused = true;
}

#endif // PLAYLIST_H
