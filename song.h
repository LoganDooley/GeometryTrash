#pragma once

#include "External/miniaudio.h"

class Song
{
public:
    Song();
    ~Song();

    ma_sound* getSongPointer(){
        return &m_song;
    }
private:
    ma_sound m_song;
};
