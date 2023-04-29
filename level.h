#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <memory>
#include <vector>
#include "player.h"
#include "aabb.h"

struct Spike{
    Spike(glm::dvec2 pos, bool flipX, bool flipY):
        m_pos(pos), m_flipX(flipX), m_flipY(flipY), m_aabb(AABB(glm::dvec2(-0.1, -0.1), glm::dvec2(0.1, 0.3), flipX, flipY))
    {

    }

    glm::dvec2 m_pos;
    bool m_flipX;
    bool m_flipY;
    AABB m_aabb;
};

enum class PortalType{
    Gravity, ReverseGravity, Cube, Ball, Ship, Wave
};

struct Portal{
    Portal(glm::dvec2 pos, PortalType portalType, bool flipX, bool flipY):
        m_pos(pos), m_portalType(portalType), m_flipX(flipX), m_flipY(flipY), m_aabb(AABB(glm::dvec2(-0.5, -1.5), glm::dvec2(0.5, 1.5), flipX, flipY))
    {

    }

    glm::dvec2 m_pos;
    PortalType m_portalType;
    bool m_flipX;
    bool m_flipY;
    AABB m_aabb;

    glm::vec3 color(){
        switch(m_portalType){
        case PortalType::Gravity:
            return glm::vec3(0.12, 0.32, 1);
            break;
        case PortalType::ReverseGravity:
            return glm::vec3(1, 1, 0);
            break;
        case PortalType::Cube:
            return glm::vec3(0.2, 1, 0.2);
            break;
        case PortalType::Ball:
            return glm::vec3(1, 0.75, 0);
            break;
        case PortalType::Ship:
            return glm::vec3(1, 0.2, 0.75);
            break;
        case PortalType::Wave:
            return glm::vec3(0.1, 0.2, 1);
            break;
        }
        return glm::vec3(1);
    }
};

enum class OrbType{
    Yellow, Pink, Red, Blue, Green, Black
};

struct Orb{
    Orb(glm::dvec2 pos, OrbType portalType, bool flipX, bool flipY):
        m_pos(pos), m_orbType(portalType), m_flipX(flipX), m_flipY(flipY), m_aabb(AABB(glm::dvec2(-0.5, -0.5), glm::dvec2(0.5, 0.5), flipX, flipY))
    {

    }

    glm::dvec2 m_pos;
    OrbType m_orbType;
    bool m_flipX;
    bool m_flipY;
    AABB m_aabb;

    glm::vec3 color(){
        switch(m_orbType){
        case OrbType::Yellow:
            return glm::vec3(1, 1, 0);
            break;
        case OrbType::Pink:
            return glm::vec3(1, 0.5, 1);
            break;
        case OrbType::Red:
            return glm::vec3(1, 0, 0);
            break;
        case OrbType::Blue:
            return glm::vec3(0, 0, 1);
            break;
        case OrbType::Green:
            return glm::vec3(0, 1, 0);
            break;
        case OrbType::Black:
            return glm::vec3(0.2, 0.2, 0.2);
            break;
        }
        return glm::vec3(1);
    }
};

class Level
{
public:
    Level();
    ~Level();

    void checkCollisions(std::shared_ptr<Player> player);
    void draw(std::shared_ptr<Player> player, glm::vec2 screenDim);

private:
    std::vector<Spike> m_spikes;
    std::vector<Portal> m_portals;
    std::vector<Orb> m_orbs;
    GLuint m_levelShader;
    GLuint m_spikeVao;
    GLuint m_spikeVbo;
    GLuint m_portalVao;
    GLuint m_portalVbo;
    GLuint m_orbVao;
    GLuint m_orbVbo;
};
