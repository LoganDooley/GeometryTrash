#pragma once

#include "aabb.h"
#include "wavetrail.h"
#include "settings.h"
#include <memory>

struct Portal;

struct Orb;

enum class Gamemode{
    Cube, Ship, Ball, Wave
};

class Player
{
public:
    Player(std::shared_ptr<Settings> settings);
    ~Player();

    void update(double dt);
    void setInput(bool input);
    void draw();

    glm::dvec2 getPos() const{
        return m_pos;
    }

    AABB getHitbox() const{
        if(m_gamemode == Gamemode::Wave){
            return m_waveHitbox;
        }
        else{
            return m_hitbox;
        }
    }

    void setGrounded(bool grounded){
        m_grounded = false;
    }

    void kill();
    void resolveCollision(double yCorrected);
    void portalInteraction(Portal portal);
    bool orbInteraction(Orb orb);

private:
    void updateCube(double dt);
    void updateShip(double dt);
    void updateBall(double dt);
    void updateWave(double dt);

    glm::vec2 getMinUV();
    glm::vec2 getUVDim();

    void flipGravity();

    void pruneWaveTrails();

    Gamemode m_gamemode = Gamemode::Cube;
    bool m_grounded = true;
    bool m_prevInput = false;
    bool m_input = false;
    bool m_orbInput = false;
    bool m_flippedGravity = false;
    glm::dvec2 m_pos = glm::dvec2(0, -4);
    glm::dvec2 m_vel = glm::dvec2(1, 0);
    glm::dvec2 m_accel = glm::dvec2(0, 0);

    GLuint m_vboCubeBall;
    GLuint m_vaoCubeBall;
    GLuint m_vboShip;
    GLuint m_vaoShip;
    GLuint m_vboWave;
    GLuint m_vaoWave;

    GLuint m_playerShader;

    AABB m_hitbox;
    AABB m_waveHitbox;
    float m_theta = 0.5;
    float m_angularVelocity = 0.75;

    glm::vec3 m_playerColor1;
    glm::vec3 m_playerColor2;

    std::vector<std::shared_ptr<WaveTrail>> m_waveTrails;

    std::shared_ptr<Settings> m_settings;
};
