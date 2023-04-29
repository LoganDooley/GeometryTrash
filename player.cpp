#include "player.h"
#include "debug.h"
#include "level.h"

#include <iostream>

Player::Player():
    m_hitbox(AABB(glm::dvec2(-0.5, -0.5), glm::dvec2(0.5, 0.5), false, false))
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
    case Gamemode::Cube:
        updateCube(dt);
        break;
    case Gamemode::Ship:
        updateShip(dt);
        break;
    case Gamemode::Ball:
        updateBall(dt);
        break;
    case Gamemode::Wave:
        updateWave(dt);
        break;
    }

    m_pos += m_vel * dt + 0.5 * m_accel * dt * dt;
    m_vel += m_accel * dt;

    if(m_pos.y < -4){
        m_pos.y = -4;
        m_vel.y = 0;
        if(!m_flippedGravity){
            m_grounded = true;
        }
    }
    else if(m_pos.y > 4){
        m_pos.y = 4;
        m_vel.y = 0;
        if(m_flippedGravity){
            m_grounded = true;
        }
    }
}

void Player::draw(glm::vec2 screenDim){
    glUseProgram(m_playerShader);
    glUniform2f(glGetUniformLocation(m_playerShader, "playerPos"), 0, m_pos.y);
    glUniform2f(glGetUniformLocation(m_playerShader, "screenDim"), screenDim.x, screenDim.y);
    glUniform2f(glGetUniformLocation(m_playerShader, "xRot"), cosf(m_theta), sinf(m_theta));
    glUniform2f(glGetUniformLocation(m_playerShader, "yRot"), -sinf(m_theta), cosf(m_theta));
    glUniform1f(glGetUniformLocation(m_playerShader, "theta"), m_theta);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
}

void Player::resolveCollision(glm::dvec2 collision){
    //std::cout<<collision.x<<" "<<collision.y<<std::endl;
    if(collision.x != 0 || collision.y != 0){
        //std::cout<<"Collision"<<std::endl;
        m_gamemode = Gamemode::Cube;
        m_grounded = true;
        m_input = false;
        m_flippedGravity = false;
        m_pos = glm::dvec2(0, -4);
        m_vel = glm::dvec2(1, 0);
        m_accel = glm::dvec2(0, 0);
    }
}

void Player::setInput(bool input){
    m_input = input;
    m_orbInput = input;
}

void Player::portalInteraction(Portal portal){
    switch(portal.m_portalType){
    case PortalType::Gravity:
        m_flippedGravity = false;
        break;
    case PortalType::ReverseGravity:
        m_flippedGravity = true;
        break;
    case PortalType::Cube:
        m_gamemode = Gamemode::Cube;
        m_theta = 0;
        break;
    case PortalType::Ship:
        m_gamemode = Gamemode::Ship;
        m_theta = 0;
        break;
    case PortalType::Ball:
        m_gamemode = Gamemode::Ball;
        m_theta = 0;
        break;
    case PortalType::Wave:
        m_gamemode = Gamemode::Wave;
        m_theta = 0;
        break;
    }
}

bool Player::orbInteraction(Orb orb){
    if(!m_orbInput){
        return false;
    }
    switch(orb.m_orbType){
    case OrbType::Yellow:
        m_vel.y = 1.91;
        if(m_flippedGravity){
            m_vel.y *= -1;
        }
        break;
    case OrbType::Pink:
        m_vel.y = 1.37;
        if(m_flippedGravity){
            m_vel.y *= -1;
        }
        break;
    case OrbType::Red:
        m_vel.y = 2.68;
        if(m_flippedGravity){
            m_vel.y *= -1;
        }
        break;
    case OrbType::Blue:
        m_vel.y = 1.37;
        if(m_flippedGravity){
            m_vel.y *= -1;
        }
        m_flippedGravity = !m_flippedGravity;
        break;
    case OrbType::Green:
        m_vel.y = -1.91;
        if(m_flippedGravity){
            m_vel.y *= -1;
        }
        m_flippedGravity = !m_flippedGravity;
        break;
    case OrbType::Black:
        m_vel.y = -2.6;
        if(m_flippedGravity){
            m_vel.y *= -1;
        }
        break;
    }
    m_orbInput = false;
    return true;
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
            m_orbInput = false;
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
            m_orbInput = false;
        }
    }

    if(!m_grounded){
        if(!m_flippedGravity){
            m_theta += m_angularVelocity * dt;
        }
        else{
            m_theta -= m_angularVelocity * dt;
        }
    }
    else{
        m_theta = M_PI/2 * (int(ceilf(m_theta/(M_PI/2))) % 4);
    }
}

void Player::updateShip(double dt){
    if(!m_flippedGravity){
        if(m_input){
            if(m_vel.y > -1.367){
                m_accel.y = 0.3755;
            }
            else{
                m_accel.y = 0;
            }
            m_orbInput = false;
        }
        else{
            if(m_vel.y < 1.125){
                m_accel.y = -0.2077;
            }
            else{
                m_accel.y = 0;
            }
        }
    }
    else{
        if(m_input){
            if(m_vel.y < 1.367){
                m_accel.y = -0.3755;
            }
            else{
                m_accel.y = 0;
            }
            m_orbInput = false;
        }
        else{
            if(m_vel.y > -1.125){
                m_accel.y = 0.2077;
            }
            else{
                m_accel.y = 0;
            }
        }
    }
}

void Player::updateBall(double dt){
    if(!m_flippedGravity){
        if(m_vel.y > -2.6){
            m_accel.y = -0.876;
        }
        else{
            m_accel.y = 0;
        }

        if(m_grounded && m_input){
            m_flippedGravity = true;
            m_grounded = false;
            m_orbInput = false;
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
            m_flippedGravity = false;
            m_grounded = false;
            m_orbInput = false;
        }
    }

    if(m_grounded){
        if(!m_flippedGravity){
            m_theta += m_angularVelocity * dt;
        }
        else{
            m_theta -= m_angularVelocity * dt;
        }
    }
    else{
        if(!m_flippedGravity){
            m_theta -= m_angularVelocity * dt;
        }
        else{
            m_theta += m_angularVelocity * dt;
        }
    }
}

void Player::updateWave(double dt){
    if(m_input){
        m_vel.y = m_vel.x;
        m_orbInput = false;
    }
    else{
        m_vel.y = -m_vel.x;
    }

    if(m_flippedGravity){
        m_vel.y = -m_vel.y;
    }
    m_accel = glm::dvec2(0, 0);
}
