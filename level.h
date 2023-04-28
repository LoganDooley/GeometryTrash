#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <memory>
#include <vector>
#include "player.h"
#include "aabb.h"

struct Spike{
    glm::dvec2 m_pos;
};

class Level
{
public:
    Level();
    ~Level();

    void checkCollisions(std::shared_ptr<Player> player);
    void draw(std::shared_ptr<Player> player);

private:
    std::vector<Spike> m_spikes;
    GLuint m_levelShader;
    GLuint m_spikeVao;
    GLuint m_spikeVbo;
    AABB m_spikeHitbox;
};
