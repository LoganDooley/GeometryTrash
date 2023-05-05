#pragma once

#include "shaderloader.h"
#include "settings.h"
#include <memory>
#include <glm/glm.hpp>
#include <vector>

class WaveTrail
{
public:
    WaveTrail(std::shared_ptr<Settings> settings, glm::dvec2 playerPos);
    ~WaveTrail();

    void addSegment(glm::dvec2 playerPos);
    void update(glm::dvec2 playerPos);

    void draw(glm::dvec2 playerPos, glm::vec3 waveColor);

    void prune(double screenLeft);

    bool isEmpty() const{
        if(m_data.size() == 0){
            return true;
        }
        return false;
    }

private:
    std::vector<GLdouble> m_data;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_waveShader;
    std::shared_ptr<Settings> m_settings;
};
