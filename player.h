#pragma once

#include <glm/glm.hpp>
#include "shaderloader.h"
#include "aabb.h"

enum Gamemode{
    Cube, Ship, Ball, Wave
};

class Player
{
public:
    Player();
    ~Player();

    void update(double dt);
    void setInput(bool input);
    void draw();

    glm::dvec2 getPos() const{
        return m_pos;
    }

    AABB getHitbox() const{
        return m_hitbox;
    }

    void resolveCollision(glm::dvec2 collision);

private:
    void updateCube(double dt);
    void updateShip(double dt);
    void updateBall(double dt);
    void updateWave(double dt);

    Gamemode m_gamemode = Cube;
    bool m_grounded = true;
    bool m_input = false;
    bool m_flippedGravity = false;
    glm::dvec2 m_pos = glm::dvec2(0, 0);
    glm::dvec2 m_vel = glm::dvec2(1, 0);
    glm::dvec2 m_accel = glm::dvec2(0, 0);

    GLuint m_vbo;
    GLuint m_vao;

    GLuint m_playerShader;

    AABB m_hitbox;
    float m_theta = 0.5;
};
