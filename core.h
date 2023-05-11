#pragma once

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include <iostream>
#include <memory>
#include "player.h"
#include "level.h"
#include "gametextures.h"
#include "settings.h"
#include "audiosystem.h"

#include <set>

class Core
{
public:
    Core(int width, int height);
    ~Core();
    int update(double seconds);
    int draw();
    void keyEvent(int key, int action);
    void mousePosEvent(double xpos, double ypos);
    void mouseButtonEvent(int button, int action);
    void scrollEvent(double distance);
    void windowResizeEvent(int width, int height);
    void framebufferResizeEvent(int width, int height);

private:
    std::shared_ptr<Player> m_player;
    std::shared_ptr<Level> m_level;
    std::shared_ptr<GameTextures> m_gameTextures;
    std::shared_ptr<Settings> m_settings;
    std::shared_ptr<AudioSystem> m_audioSystem;

    std::set<int> m_inputsDown = std::set<int>();
};
