#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <memory>
#include <vector>
#include "collision.h"
#include "settings.h"

class Player;

struct Spike{
    Spike(glm::dvec2 pos, bool flipX, bool flipY):
        m_pos(pos), m_flipX(flipX), m_flipY(flipY), m_aabb({flipX, flipY, 1, glm::dvec2(-3, -5.75), glm::dvec2(3, 5.75)})
    {

    }

    glm::dvec2 m_pos;
    bool m_flipX;
    bool m_flipY;
    AABB m_aabb;
};

struct Block{
    Block(glm::dvec2 pos, bool flipX, bool flipY):
        m_pos(pos), m_flipX(flipX), m_flipY(flipY), m_aabb({flipX, flipY, 1, glm::dvec2(-15, -15), glm::dvec2(15, 15)})
    {

    }

    glm::dvec2 m_pos;
    bool m_flipX;
    bool m_flipY;
    AABB m_aabb;
};

struct Slope1{
    Slope1(glm::dvec2 pos, bool flipX, bool flipY):
        m_pos(pos), m_flipX(flipX), m_flipY(flipY), m_slope45({flipX, flipY, 1})
    {

    }

    glm::dvec2 m_pos;
    bool m_flipX;
    bool m_flipY;
    Slope45 m_slope45;
};

struct Slope2{
    Slope2(glm::dvec2 pos, bool flipX, bool flipY):
        m_pos(pos), m_flipX(flipX), m_flipY(flipY), m_slope60({flipX, flipY, 1})
    {

    }

    glm::dvec2 m_pos;
    bool m_flipX;
    bool m_flipY;
    Slope60 m_slope60;
};

enum class PortalType{
    Gravity, ReverseGravity, MiniSize, NormalSize, Cube, Ball, Ship, Wave, UFO
};

struct Portal{
    Portal(glm::dvec2 pos, PortalType portalType, bool flipX, bool flipY):
        m_pos(pos), m_portalType(portalType), m_flipX(flipX), m_flipY(flipY), m_aabb({flipX, flipY, 1, glm::dvec2(-15, -45), glm::dvec2(15, 45)})
    {

    }

    glm::dvec2 m_pos;
    PortalType m_portalType;
    bool m_flipX;
    bool m_flipY;
    bool m_activated = false;
    AABB m_aabb;

    glm::vec3 color(){
        switch(m_portalType){
        case PortalType::Gravity:
            return glm::vec3(0.443137, 0.945098, 0.996078);
            break;
        case PortalType::ReverseGravity:
            return glm::vec3(1, 0.992157, 0.25098);
            break;
        case PortalType::MiniSize:
            return glm::vec3(0.964706, 0.239216, 0.894118);
            break;
        case PortalType::NormalSize:
            return glm::vec3(0.0156863, 0.984314, 0.027451);
            break;
        case PortalType::Cube:
            return glm::vec3(0.317647, 1, 0.313725);
            break;
        case PortalType::Ball:
            return glm::vec3(0.921569, 0.352941, 0.188235);
            break;
        case PortalType::Ship:
            return glm::vec3(0.937255, 0.505882, 0.964706);
            break;
        case PortalType::Wave:
            return glm::vec3(0.180392, 0.854902, 0.980392);
            break;
        case PortalType::UFO:
            return glm::vec3(0.996078, 0.721569, 0.0470588);
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
        m_pos(pos), m_orbType(portalType), m_flipX(flipX), m_flipY(flipY), m_aabb({flipX, flipY, 1, glm::dvec2(-0.5, -0.5), glm::dvec2(0.5, 0.5)})
    {

    }

    glm::dvec2 m_pos;
    OrbType m_orbType;
    bool m_flipX;
    bool m_flipY;
    bool m_activated = false;
    AABB m_aabb;

    glm::vec3 color(){
        switch(m_orbType){
        case OrbType::Yellow:
            return glm::vec3(1, 1, 0.403922);
            break;
        case OrbType::Pink:
            return glm::vec3(0.988235, 0.309804, 0.988235);
            break;
        case OrbType::Red:
            return glm::vec3(0.996078, 0.309804, 0.176471);
            break;
        case OrbType::Blue:
            return glm::vec3(0.282353, 1, 1);
            break;
        case OrbType::Green:
            return glm::vec3(0.396078, 0.984314, 0.254902);
            break;
        case OrbType::Black:
            return glm::vec3(0.0588235, 0.0627451, 0.0666667);
            break;
        }
        return glm::vec3(1);
    }
};

enum class PadType{
    Yellow, Pink, Red, Blue
};

struct Pad{
    Pad(glm::dvec2 pos, PadType padType, bool flipX, bool flipY):
        m_pos(pos), m_padType(padType), m_flipX(flipX), m_flipY(flipY), m_aabb({flipX, flipY, 1, glm::dvec2(-0.5, -0.5), glm::dvec2(0.5, -0.25)})
    {

    }

    glm::dvec2 m_pos;
    PadType m_padType;
    bool m_flipX;
    bool m_flipY;
    bool m_activated = false;
    AABB m_aabb;

    glm::vec3 color(){
        switch(m_padType){
        case PadType::Yellow:
            return glm::vec3(1, 1, 0.403922);
            break;
        case PadType::Pink:
            return glm::vec3(0.988235, 0.309804, 0.988235);
            break;
        case PadType::Red:
            return glm::vec3(0.996078, 0.309804, 0.176471);
            break;
        case PadType::Blue:
            return glm::vec3(0.282353, 1, 1);
            break;
        }
        return glm::vec3(1);
    }
};

class Level
{
public:
    Level(std::shared_ptr<Settings> settings);
    ~Level();

    void draw(std::shared_ptr<Player> player);

    std::vector<Spike>& getSpikes(){
        return m_spikes;
    }

    std::vector<Block>& getBlocks(){
        return m_blocks;
    }

    std::vector<Portal>& getPortals(){
        return m_portals;
    }

    std::vector<Orb>& getOrbs(){
        return m_orbs;
    }

    std::vector<Pad>& getPads(){
        return m_pads;
    }

    std::vector<Slope1>& getSlope1s(){
        return m_slope1s;
    }

    std::vector<Slope2>& getSlope2s(){
        return m_slope2s;
    }

private:
    std::vector<Spike> m_spikes;
    std::vector<Block> m_blocks;
    std::vector<Portal> m_portals;
    std::vector<Orb> m_orbs;
    std::vector<Pad> m_pads;
    std::vector<Slope1> m_slope1s;
    std::vector<Slope2> m_slope2s;

    GLuint m_levelShader;
    GLuint m_spikeVao;
    GLuint m_spikeVbo;
    GLuint m_blockVao;
    GLuint m_blockVbo;
    GLuint m_portalVao;
    GLuint m_portalVbo;
    GLuint m_orbVao;
    GLuint m_orbVbo;
    GLuint m_padVao;
    GLuint m_padVbo;
    GLuint m_slope1Vao;
    GLuint m_slope1Vbo;
    GLuint m_slope2Vao;
    GLuint m_slope2Vbo;

    std::shared_ptr<Settings> m_settings;
};
