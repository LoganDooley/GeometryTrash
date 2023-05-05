#pragma once

#include <glad/glad.h>

class GameTextures
{
public:
    GameTextures();
    ~GameTextures();

    void bindIconTextures();
    void unbindIconTextures();

private:
    GLuint m_iconTexture;
};
