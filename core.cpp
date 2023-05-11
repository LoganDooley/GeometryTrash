#include "core.h"

#include "debug.h"

Core::Core(int width, int height):
    m_settings(std::make_shared<Settings>(width, height, 165, 0))
{
    glViewport(0, 0, width, height);
    Debug::checkGLError();
    m_player = std::make_shared<Player>(m_settings);
    m_level = std::make_shared<Level>(m_settings);
    m_gameTextures = std::make_shared<GameTextures>();

    glEnable(GL_BLEND);
}

Core::~Core(){

}

int Core::update(double seconds){
    m_player->update(seconds);
    m_player->collideWithLevel(m_level);
    return 0;
}

int Core::draw(){
    glClearColor(0.4, 0.4, 0.4, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_gameTextures->bindIconTextures();
    m_player->draw();
    m_gameTextures->unbindIconTextures();
    m_level->draw(m_player);
    return 0;
}

void Core::keyEvent(int key, int action){
    if(key == GLFW_KEY_SPACE || key == GLFW_KEY_UP || key == GLFW_MOUSE_BUTTON_LEFT){
        if(action == GLFW_PRESS){
            m_player->addInputs(1);
            m_player->setInputDown(true);
            m_inputsDown.insert(key);
        }
        else if(action == GLFW_RELEASE){
            m_player->removeInputs(1);
            m_inputsDown.erase(key);
            if(m_inputsDown.size() == 0){
                m_player->setInputDown(false);
            }
        }
    }
}

void Core::mousePosEvent(double xpos, double ypos){

}

void Core::mouseButtonEvent(int button, int action){

}

void Core::scrollEvent(double distance){
    
}

void Core::framebufferResizeEvent(int width, int height){
    glViewport(0, 0, width, height);
}

void Core::windowResizeEvent(int width, int height){
    m_settings->m_width = width;
    m_settings->m_height = height;
}
