#include "player.h"
#include "debug.h"
#include "level.h"

#include <iostream>

Player::Player(std::shared_ptr<Settings> settings):
    m_hitbox(AABB(glm::dvec2(-0.5, -0.5), glm::dvec2(0.5, 0.5), false, false)),
    m_waveHitbox(AABB(glm::dvec2(-0.125, -0.125), glm::dvec2(0.125, 0.125), false, false)),
    m_settings(settings)
{
    GLdouble dataCubeBall[24] = {
        -0.5, -0.5,
        0, 0,
        0.5, -0.5,
        1, 0,
        0.5, 0.5,
        1, 1,
        0.5, 0.5,
        1, 1,
        -0.5, 0.5,
        0, 1,
        -0.5, -0.5,
        0, 0
    };

    glGenVertexArrays(1, &m_vaoCubeBall);
    glBindVertexArray(m_vaoCubeBall);
    glGenBuffers(1, &m_vboCubeBall);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboCubeBall);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dataCubeBall), dataCubeBall, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(GLdouble), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(GLdouble), (void*)(2 * sizeof(GLdouble)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLdouble dataShip[24] = {
        -1, -0.5,
        0, 0,
        1, -0.5,
        1, 0,
        1, 0.5,
        1, 1,
        1, 0.5,
        1, 1,
        -1, 0.5,
        0, 1,
        -1, -0.5,
        0, 0
    };

    glGenVertexArrays(1, &m_vaoShip);
    glBindVertexArray(m_vaoShip);
    glGenBuffers(1, &m_vboShip);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboShip);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dataShip), dataShip, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(GLdouble), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(GLdouble), (void*)(2 * sizeof(GLdouble)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLdouble dataWave[24] = {
        -0.25, -0.25,
        0, 0,
        0.25, -0.25,
        1, 0,
        0.25, 0.25,
        1, 1,
        0.25, 0.25,
        1, 1,
        -0.25, 0.25,
        0, 1,
        -0.25, -0.25,
        0, 0
    };

    glGenVertexArrays(1, &m_vaoWave);
    glBindVertexArray(m_vaoWave);
    glGenBuffers(1, &m_vboWave);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboWave);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dataWave), dataWave, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(GLdouble), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(GLdouble), (void*)(2 * sizeof(GLdouble)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_playerShader = ShaderLoader::createShaderProgram("Shaders/player.vert", "Shaders/player.frag");
    Debug::checkGLError();

    m_playerColor1 = glm::vec3(0, 1, 0);
    m_playerColor2 = glm::vec3(0, 0, 1);
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

    m_prevInput = m_input;

    pruneWaveTrails();
}

void Player::draw(){
    for(int i = 0; i<m_waveTrails.size(); i++){
        m_waveTrails.at(i)->draw(m_pos, m_playerColor1);
    }
    glUseProgram(m_playerShader);
    glUniform2f(glGetUniformLocation(m_playerShader, "playerPos"), 0, m_pos.y);
    glUniform2f(glGetUniformLocation(m_playerShader, "screenDim"), m_settings->m_width, m_settings->m_height);
    glUniform1f(glGetUniformLocation(m_playerShader, "yUnits"), m_settings->m_yUnits);
    glUniform2f(glGetUniformLocation(m_playerShader, "xRot"), cosf(m_theta), sinf(m_theta));
    glUniform2f(glGetUniformLocation(m_playerShader, "yRot"), -sinf(m_theta), cosf(m_theta));
    glUniform1f(glGetUniformLocation(m_playerShader, "theta"), m_theta);
    glUniform2f(glGetUniformLocation(m_playerShader, "minUV"), getMinUV().x, getMinUV().y);
    glUniform2f(glGetUniformLocation(m_playerShader, "UVDim"), getUVDim().x, getUVDim().y);
    glUniform3f(glGetUniformLocation(m_playerShader, "playerColor1"), m_playerColor1.r, m_playerColor1.g, m_playerColor1.b);
    glUniform3f(glGetUniformLocation(m_playerShader, "playerColor2"), m_playerColor2.r, m_playerColor2.g, m_playerColor2.b);
    switch(m_gamemode){
    case Gamemode::Cube:
        glBindVertexArray(m_vaoCubeBall);
        break;
    case Gamemode::Ball:
        glBindVertexArray(m_vaoCubeBall);
        break;
    case Gamemode::Ship:
        glBindVertexArray(m_vaoShip);
        break;
    case Gamemode::Wave:
        glBindVertexArray(m_vaoWave);
        break;
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
}

void Player::kill(){
    m_gamemode = Gamemode::Cube;
    m_grounded = true;
    m_input = false;
    m_flippedGravity = false;
    m_pos = glm::dvec2(0, -4);
    m_vel = glm::dvec2(1, 0);
    m_accel = glm::dvec2(0, 0);
}

void Player::resolveCollision(double yCorrected){
    if(!m_flippedGravity){
        if(yCorrected > m_pos.y){
            m_grounded = true;
            m_vel.y = 0;
            m_pos.y = yCorrected;
            if(m_gamemode == Gamemode::Wave){
                m_waveTrails.at(m_waveTrails.size()-1)->addSegment(m_pos);
            }
        }
        else if(m_vel.y > 0.1){
            kill();
        }
    }
    else{
        if(yCorrected < m_pos.y){
            m_grounded = true;
            m_vel.y = 0;
            m_pos.y = yCorrected;
            if(m_gamemode == Gamemode::Wave){
                m_waveTrails.at(m_waveTrails.size()-1)->addSegment(m_pos);
            }
        }
        else if(m_vel.y < -0.1){
            kill();
        }
    }
}

void Player::setInput(bool input){
    m_prevInput = m_input;
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
        m_theta = atan2f(-m_vel.y, m_vel.x);
        break;
    case PortalType::Ball:
        m_gamemode = Gamemode::Ball;
        m_theta = 0;
        break;
    case PortalType::Wave:
        if(m_gamemode != Gamemode::Wave){
            m_gamemode = Gamemode::Wave;
            if(m_vel.y > 0){
                m_theta = M_PI/4;
            }
            else if(m_vel.y < 0){
                m_theta = 3 * M_PI/4;
            }
            m_waveTrails.push_back(std::make_shared<WaveTrail>(m_settings, m_pos));
        }
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
        flipGravity();
        break;
    case OrbType::Green:
        m_vel.y = -1.91;
        if(m_flippedGravity){
            m_vel.y *= -1;
        }
        flipGravity();
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

    m_theta = atan2f(-m_vel.y, m_vel.x);
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
            flipGravity();
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
            flipGravity();
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

    if(m_vel.y > 0){
        m_theta = M_PI/4;
    }
    else if(m_vel.y < 0){
        m_theta = 3 * M_PI/4;
    }
    else{
        m_theta = 0;
    }

    if(m_input != m_prevInput){
        m_waveTrails.at(m_waveTrails.size()-1)->addSegment(m_pos);
    }
    m_waveTrails.at(m_waveTrails.size()-1)->update(m_pos);
}

glm::vec2 Player::getMinUV(){
    switch(m_gamemode){
    case Gamemode::Cube:
        return glm::vec2(0, 1 - 32.f/352);
        break;
    case Gamemode::Ball:
        return glm::vec2(0, 1 - (32.f+1.f+32.f)/352);
        break;
    case Gamemode::Wave:
        return glm::vec2(0, 1 - (32.f+1.f+32.f+1.f+32.f)/352);
        break;
    case Gamemode::Ship:
        return glm::vec2(0, 1 - (32.f+1.f+32.f+1.f+32.f+1.f+32.f)/352);
        break;
    }
}

glm::vec2 Player::getUVDim(){
    switch(m_gamemode){
    case Gamemode::Cube:
        return glm::vec2(32.f/352, 32.f/352);
        break;
    case Gamemode::Ball:
        return glm::vec2(32.f/352, 32.f/352);
        break;
    case Gamemode::Wave:
        return glm::vec2(32.f/352, 32.f/352);
        break;
    case Gamemode::Ship:
        return glm::vec2(64.f/352, 32.f/352);
        break;
    }
}

void Player::flipGravity(){
    m_flippedGravity = !m_flippedGravity;
    if(m_gamemode == Gamemode::Wave){
        m_waveTrails.at(m_waveTrails.size()-1)->addSegment(m_pos);
    }
}

void Player::pruneWaveTrails(){
    for(int i = 0; i<m_waveTrails.size(); i++){
        m_waveTrails.at(0)->prune(m_pos.x - (m_settings->m_yUnits * m_settings->m_width)/m_settings->m_height);
        if(!m_waveTrails.at(0)->isEmpty()){
            break;
        }
        else{
            m_waveTrails.erase(m_waveTrails.begin());
        }
    }
}
