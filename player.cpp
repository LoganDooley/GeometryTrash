#include "player.h"
#include "debug.h"

#include <iostream>

Player::Player():
    m_hitbox(AABB(glm::dvec2(-0.5, -0.5), glm::dvec2(0.5, 0.5)))
{
    GLdouble data[12] = {
        -0.5, -0.5,
        0.5, -0.5,
        0.5, 0.5,
        0.5, 0.5,
        -0.5, 0.5,
        -0.5, -0.5
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_playerShader = ShaderLoader::createShaderProgram("Shaders/player.vert", "Shaders/player.frag");
    Debug::checkGLError();
}

Player::~Player(){

}

void Player::update(double dt){
    switch(m_gamemode){
    case Cube:
        updateCube(dt);
        break;
    case Ship:
        updateShip(dt);
        break;
    case Ball:
        updateBall(dt);
        break;
    case Wave:
        updateWave(dt);
        break;
    }

    if(m_pos.y < 0){
        m_pos.y = 0;
        m_vel.y = 0;
        m_grounded = true;
    }

    m_theta += dt;
}

void Player::draw(){
    glUseProgram(m_playerShader);
    glUniform2f(glGetUniformLocation(m_playerShader, "playerPos"), 0, m_pos.y);
    glUniform2f(glGetUniformLocation(m_playerShader, "screenDim"), 6.4, 4.8);
    glUniform1f(glGetUniformLocation(m_playerShader, "theta"), m_theta);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
}

void Player::resolveCollision(glm::dvec2 collision){
    std::cout<<collision.x<<" "<<collision.y<<std::endl;
    if(collision.x != 0 || collision.y != 0){
        std::cout<<"Collision"<<std::endl;
        m_gamemode = Cube;
        m_grounded = true;
        m_input = false;
        m_flippedGravity = false;
        m_pos = glm::dvec2(0, 0);
        m_vel = glm::dvec2(1, 0);
        m_accel = glm::dvec2(0, 0);
    }
}

void Player::setInput(bool input){
    m_input = input;
}

void Player::updateCube(double dt){
    if(!m_flippedGravity){
        if(m_vel.y > -2.6){
            m_accel.y = -0.876;
        }
        else{
            m_accel.y = 0;
        }

        if(m_grounded && m_input){
            m_vel.y = 1.94;
            m_grounded = false;
        }
    }
    else{
        if(m_vel.y < 2.6){
            m_accel.y = 0.876;
        }
        else{
            m_accel.y = 0;
        }

        if(m_grounded && m_input){
            m_vel.y = -1.94;
            m_grounded = false;
        }
    }

    m_pos += m_vel * dt + 0.5 * m_accel * dt * dt;
    m_vel += m_accel * dt;
}

void Player::updateShip(double dt){

}

void Player::updateBall(double dt){

}

void Player::updateWave(double dt){
    if(m_input){
        m_vel.y = m_vel.x;
    }
    else{
        m_vel.y = -m_vel.x;
    }

    if(m_flippedGravity){
        m_vel.y = -m_vel.y;
    }
}
