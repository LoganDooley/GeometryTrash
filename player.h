#pragma once

#include "collision.h"
#include "wavetrail.h"
#include "settings.h"
#include <memory>

#include "level.h"

struct Portal;

struct Orb;

enum class Gamemode{
    Cube, Ship, Ball, Wave, UFO
};

class Player
{
public:
    Player(std::shared_ptr<Settings> settings);
    ~Player();

    void update(double dt);

    void setInputDown(bool inputDown){
        m_inputDown = inputDown;
    }
    void addInputs(int numInputs){
        m_inputs += numInputs;
    }
    void removeInputs(int numInputs){
        m_inputs -= numInputs;
        if(m_inputs < 0){
            m_inputs = 0;
        }
    }

    void setInput(bool input);
    void draw();

    glm::dvec2 getPos() const{
        return m_pos;
    }

    AABB getOuterHitbox() const{
        if(m_gamemode == Gamemode::Wave){
            return m_waveHitbox;
        }
        else{
            return m_outerHitbox;
        }
    }

    Circle getSlopeHitbox() const{
        return m_slopeHitbox;
    }

    void setGrounded(bool grounded){
        m_grounded = false;
    }

    void kill();
    void resolveCollision(CollisionData data);
    void portalInteraction(Portal& portal);
    bool orbInteraction(Orb& orb);
    void padInteraction(Pad& pad);

    void collideWithLevel(std::shared_ptr<Level> level);

private:
    void updateCube(double dt);
    void updateShip(double dt);
    void updateBall(double dt);
    void updateWave(double dt);
    void updateUFO(double dt);

    glm::vec2 getMinUV();
    glm::vec2 getUVDim();

    void flipGravity();

    void pruneWaveTrails();

    bool resolveBoxCollision(CollisionData data);

    Gamemode m_gamemode = Gamemode::Cube;
    bool m_grounded = true;
    bool m_inputDown = false;
    int m_inputs = 0;
    bool m_flippedGravity = false;
    bool m_mini = false;
    glm::dvec2 m_pos = glm::dvec2(0, -120);
    glm::dvec2 m_vel = glm::dvec2(311.5, 0);
    glm::dvec2 m_accel = glm::dvec2(0, 0);

    GLuint m_vboCubeBall;
    GLuint m_vaoCubeBall;
    GLuint m_vboShip;
    GLuint m_vaoShip;
    GLuint m_vboWave;
    GLuint m_vaoWave;

    GLuint m_playerShader;

    AABB m_outerHitbox;
    AABB m_innerHitbox;
    Circle m_slopeHitbox;
    AABB m_waveHitbox;
    float m_theta = 0.5;
    float m_angularVelocity = 7.2498321;

    glm::vec3 m_playerColor1;
    glm::vec3 m_playerColor2;

    std::vector<std::shared_ptr<WaveTrail>> m_waveTrails;

    std::shared_ptr<Settings> m_settings;
};
