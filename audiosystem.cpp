#include "audiosystem.h"

#define MINIAUDIO_IMPLEMENTATION
#include "External/miniaudio.h"

AudioSystem::AudioSystem()
{
    // MINIAUDIO setup

    ma_result result;

    result = ma_engine_init(NULL, &engine);
    if(result != MA_SUCCESS){
        printf("Failed to initialize audio engine");
    }
}

AudioSystem::~AudioSystem()
{
    ma_engine_uninit(&engine);
}

int AudioSystem::initSong(std::string songFile, std::shared_ptr<Song> song){
    ma_result result;

    result = ma_sound_init_from_file(&engine, songFile.c_str(), 0, NULL, NULL, song->getSongPointer());
    if(result != MA_SUCCESS){
        return result;
    }
    return 0;
}
