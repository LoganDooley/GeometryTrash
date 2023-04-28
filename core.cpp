#include "core.h"

#include "debug.h"

Core::Core(int width, int height):
    m_screenResolution(glm::ivec2(width, height))
{
    m_shader = ShaderLoader::createShaderProgram("Shaders/gerstnerWave.vert", "Shaders/gerstnerWave.frag");
    Debug::checkGLError();
    m_camera = std::make_unique<Camera>(width, height, glm::vec3(0, 5, -5), glm::vec3(0, -1, 1), glm::vec3(0, 1, 0), 1.f, 0.1f, 100.f);
    Debug::checkGLError();
    glViewport(0, 0, width, height);
    Debug::checkGLError();
    m_player = std::make_shared<Player>();
    m_level = std::make_shared<Level>();
}

Core::~Core(){

}

int Core::update(double seconds){
    m_player->update(seconds * m_dtMultiplier);
    m_level->checkCollisions(m_player);
    return 0;
}

int Core::draw(){
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_player->draw(glm::vec2(m_screenResolution.x * m_unitsY/m_screenResolution.y, m_unitsY));
    m_level->draw(m_player, glm::vec2(m_screenResolution.x * m_unitsY/m_screenResolution.y, m_unitsY));
    return 0;
}

void Core::keyEvent(int key, int action){
    if(key == GLFW_KEY_SPACE || key == GLFW_KEY_UP){
        if(action == GLFW_PRESS){
            m_player->setInput(true);
        }
        else if(action == GLFW_RELEASE){
            m_player->setInput(false);
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
    m_screenResolution = glm::ivec2(width, height);
}
