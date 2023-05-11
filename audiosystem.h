#pragma once

#include "song.h"
#include <stdio.h>
#include <memory>
#include <string>

class AudioSystem
{
public:
    AudioSystem();
    ~AudioSystem();

    int initSong(std::string songFile, std::shared_ptr<Song> song);
private:
    ma_engine engine;
};
