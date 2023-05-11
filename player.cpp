#include "player.h"
#include "debug.h"
#include "level.h"

#include <iostream>

Player::Player(std::shared_ptr<Settings> settings):
    m_outerHitbox({false, false, 1, glm::dvec2(-15, -15), glm::dvec2(15, 15)}),
    m_innerHitbox({false, false, 1, glm::dvec2(-7, -7), glm::dvec2(7, 7)}),
    m_waveHitbox({false, false, 1, glm::dvec2(-7, -7), glm::dvec2(7, 7)}),
    m_slopeHitbox({false, false, 1}),
    m_settings(settings)
{
    GLdouble dataCubeBall[24] = {
        -15, -15,
        0, 0,
        15, -15,
        1, 0,
        15, 15,
        1, 1,
        15, 15,
        1, 1,
        -15, 15,
        0, 1,
        -15, -15,
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
        -30, -15,
        0, 0,
        30, -15,
        1, 0,
        30, 15,
        1, 1,
        30, 15,
        1, 1,
        -30, 15,
        0, 1,
        -30, -15,
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
        -8, -8,
        0, 0,
        8, -8,
        1, 0,
        8, 8,
        1, 1,
        8, 8,
        1, 1,
        -8, 8,
        0, 1,
        -8, -8,
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
    case Gamemode::UFO:
        updateUFO(dt);
        break;
    }

    m_pos += m_vel * dt + 0.5 * m_accel * dt * dt;
    m_vel += m_accel * dt;

    //m_prevInput = m_input;

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
    glUniform1i(glGetUniformLocation(m_playerShader, "mini"), m_mini);
    switch(m_gamemode){
    case Gamemode::Cube:
        glBindVertexArray(m_vaoCubeBall);
        break;
    case Gamemode::Ball:
        glBindVertexArray(m_vaoCubeBall);
        break;
    case Gamemode::UFO:
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
    m_inputs = 0;
    m_flippedGravity = false;
    m_pos = glm::dvec2(0, -120);
    m_vel = glm::dvec2(311.5, 0);
    m_accel = glm::dvec2(0, 0);
    m_mini = false;
}

void Player::resolveCollision(CollisionData data){
    if(!m_flippedGravity){
        if(data.m_yCorrected > m_pos.y){
            m_grounded = true;
            m_vel.y = m_vel.x * data.m_slope;
            m_pos.y = data.m_yCorrected;
            if(m_gamemode == Gamemode::Wave){
                m_waveTrails.at(m_waveTrails.size()-1)->addSegment(m_pos);
            }
        }
        else if(m_vel.y > 0.1){
            kill();
        }
    }
    else{
        if(data.m_yCorrected < m_pos.y){
            m_grounded = true;
            m_vel.y = m_vel.x * data.m_slope;
            m_pos.y = data.m_yCorrected;
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
    //m_prevInput = m_input;
    //m_input = input;
    //m_orbInput = input;
}

void Player::portalInteraction(Portal& portal){
    switch(portal.m_portalType){
    case PortalType::Gravity:
        m_flippedGravity = false;
        break;
    case PortalType::ReverseGravity:
        m_flippedGravity = true;
        break;
    case PortalType::MiniSize:
        m_mini = true;
        break;
    case PortalType::NormalSize:
        m_mini = false;
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
    case PortalType::UFO:
        if(m_gamemode != Gamemode::UFO){
            m_gamemode = Gamemode::UFO;
            m_theta = 0;
            break;
        }
    }
    //portal.m_activated = true;
}

bool Player::orbInteraction(Orb& orb){
    if(m_inputs == 0){
        return false;
    }
    switch(orb.m_orbType){
    case OrbType::Yellow:
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 603.72;
            }
            else{
                m_vel.y = 436.4064;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 394.6644;
            }
            else{
                m_vel.y = 310.122;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 432;
            }
            else{
                m_vel.y = 504.9;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 411.048;
            }
            else{
                m_vel.y = 450.1062;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case OrbType::Pink:
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 434.7;
            }
            else{
                m_vel.y = 347.76;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 297.4644;
            }
            else{
                m_vel.y = 232.3836;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 242.0064;
            }
            else{
                m_vel.y = 200.61;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 225.6228;
            }
            else{
                m_vel.y = 169.9812;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case OrbType::Red:
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 786.564;
            }
            else{
                m_vel.y = 619.9416;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 538.3476;
            }
            else{
                m_vel.y = 425.0934;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 588.8214;
            }
            else{
                m_vel.y = 658.638;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 587.8548;
            }
            else{
                m_vel.y = 623.9754;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case OrbType::Blue:
        flipGravity();
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 288.0522;
            }
            else{
                m_vel.y = 239.76;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 196.9812;
            }
            else{
                m_vel.y = 163.1772;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 218.2032;
            }
            else{
                m_vel.y = 165.7962;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 260.118;
            }
            else{
                m_vel.y = 215.1036;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case OrbType::Green:
        flipGravity();
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = -557.1558;
            }
            else{
                m_vel.y = -436.4064;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = -394.6644;
            }
            else{
                m_vel.y = -310.1436;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = -432;
            }
            else{
                m_vel.y = -360;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = -411.0426;
            }
            else{
                m_vel.y = -450.1062;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case OrbType::Black:
        switch(m_gamemode){
        case Gamemode::Cube:
            m_vel.y = -810;
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            m_vel.y = -810;
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = -732.7152;
            }
            else{
                m_vel.y = -728.6058;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = -623.43;
            }
            else{
                m_vel.y = -626.7132;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    }
    m_inputs -= 1;
    //orb.m_activated = true;
    return true;
}

void Player::padInteraction(Pad& pad){
    switch(pad.m_padType){
    case PadType::Yellow:
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 603.18;
            }
            else{
                m_vel.y = 691.2;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 511.4124;
            }
            else{
                m_vel.y = 414.72;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 432;
            }
            else{
                m_vel.y = 691.2;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 432;
            }
            else{
                m_vel.y = 691.2;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case PadType::Pink:
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 549.9576;
            }
            else{
                m_vel.y = 449.28;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 355.86;
            }
            else{
                m_vel.y = 290.304;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 296.811;
            }
            else{
                m_vel.y = 241.92;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 338.58;
            }
            else{
                m_vel.y = 276.48;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case PadType::Red:
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 1056.7152;
            }
            else{
                m_vel.y = 840.7152;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 634.0302;
            }
            else{
                m_vel.y = 504.4302;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 533.142;
            }
            else{
                m_vel.y = 643.491;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 504.4302;
            }
            else{
                m_vel.y = 660.9384;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    case PadType::Blue:
        flipGravity();
        switch(m_gamemode){
        case Gamemode::Cube:
            if(!m_mini){
                m_vel.y = 357.24186;
            }
            else{
                m_vel.y = 276.48;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ball:
            if(!m_mini){
                m_vel.y = 214.3476;
            }
            else{
                m_vel.y = 165.888;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Ship:
            if(!m_mini){
                m_vel.y = 345.6;
            }
            else{
                m_vel.y = 276.48;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::UFO:
            if(!m_mini){
                m_vel.y = 345.6;
            }
            else{
                m_vel.y = 276.48;
            }
            if(m_flippedGravity){
                m_vel.y *= -1;
            }
            break;
        case Gamemode::Wave:
            break;
        }
        break;
    }
    //pad.m_activated = true;
}

void Player::updateCube(double dt){
    double minYVelocity = -810;
    double accel = -2794.1;
    double jumpYVelocity = 603.18;
    if(m_mini){
        jumpYVelocity = 480.6;
    }

    m_accel.y = 0;
    if((m_flippedGravity && m_vel.y < -minYVelocity) || (!m_flippedGravity && m_vel.y > minYVelocity)){
        m_accel.y = accel;
    }
    if(m_grounded && m_inputDown){
        m_vel.y = jumpYVelocity;
        m_grounded = false;
        m_inputs = 0;
    }
    if(m_flippedGravity){
        m_vel.y *= -1;
        m_accel.y *= -1;
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
    double minYVelocity = -345.6;
    double maxYVelocity = 432;
    double gravity = -1378.4267;
    double accel = 1117.64315;
    if(m_mini){
        minYVelocity = -406.588;
        maxYVelocity = 508.23504;
        gravity = -1051.9;
        accel = 854.103;
    }

    if(!m_flippedGravity){
        if(m_inputDown){
            if(m_vel.y < maxYVelocity){
                m_accel.y = accel;
            }
            else{
                m_accel.y = 0;
            }
            m_inputs = 0;
        }
        else{
            if(m_vel.y > minYVelocity){
                m_accel.y = gravity;
            }
            else{
                m_accel.y = 0;
            }
        }
    }
    else{
        if(m_inputDown){
            if(m_vel.y > -maxYVelocity){
                m_accel.y = -accel;
            }
            else{
                m_accel.y = 0;
            }
            m_inputs = 0;
        }
        else{
            if(m_vel.y < -minYVelocity){
                m_accel.y = -gravity;
            }
            else{
                m_accel.y = 0;
            }
        }
    }

    m_theta = atan2f(-m_vel.y, m_vel.x);
}

void Player::updateBall(double dt){
    double minYVelocity = -810;
    double gravity = -1676.46;
    double switchYVelocity = 181.955;
    if(m_mini){
        switchYVelocity = 145.452;
    }

    if(!m_flippedGravity){
        if(m_vel.y > minYVelocity){
            m_accel.y = gravity;
        }
        else{
            m_accel.y = 0;
        }

        if(m_grounded && m_inputDown){
            flipGravity();
            m_accel.y = -gravity;
            m_vel.y = switchYVelocity;
            m_grounded = false;
            m_inputs = 0;
        }
    }
    else{
        if(m_vel.y < -minYVelocity){
            m_accel.y = -gravity;
        }
        else{
            m_accel.y = 0;
        }

        if(m_grounded && m_inputDown){
            flipGravity();
            m_accel.y = gravity;
            m_vel.y = -switchYVelocity;
            m_grounded = false;
            m_inputs = 0;
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
    double yVelocity = m_vel.x;
    if(m_mini){
        yVelocity = 2 * m_vel.x;
    }

    double prevVel = m_vel.y;
    if(m_inputDown){
        m_vel.y = yVelocity;
        m_inputs = 0;
    }
    else{
        m_vel.y = -yVelocity;
    }

    if(m_flippedGravity){
        m_vel.y *= -1;
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

    if(prevVel != m_vel.y){
        m_waveTrails.at(m_waveTrails.size()-1)->addSegment(m_pos);
    }

    m_waveTrails.at(m_waveTrails.size()-1)->update(m_pos);
}

void Player::updateUFO(double dt){
    double minYVelocity = -345.6;
    double jumpYVelocity = 376.92;
    double gravity = -1117.6407;
    if(m_mini){
        minYVelocity = -406.588;
        jumpYVelocity = 366.12;
        gravity = -1314.8802;
    }

    if(!m_flippedGravity){
        if(m_vel.y > minYVelocity){
            m_accel.y = gravity;
        }
        else{
            m_accel.y = 0;
        }

        if(m_inputDown && m_inputs > 0){
            m_vel.y = jumpYVelocity;
            m_inputs -= 1;
        }
    }
    else{
        if(m_vel.y < -minYVelocity){
            m_accel.y = -gravity;
        }
        else{
            m_accel.y = 0;
        }

        if(m_inputDown && m_inputs > 0){
            m_vel.y = -jumpYVelocity;
            m_inputs -= 1;
        }
    }
}

glm::vec2 Player::getMinUV(){
    switch(m_gamemode){
    case Gamemode::Cube:
        return glm::vec2(0, 1 - 32.f/352);
        break;
    case Gamemode::Ball:
        return glm::vec2(0, 1 - (32.f+1.f+32.f)/352);
        break;
    case Gamemode::UFO:
        return glm::vec2(0, 1 - 32.f/352);
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
    case Gamemode::UFO:
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

void Player::collideWithLevel(std::shared_ptr<Level> level){
    std::vector<Spike> spikes = level->getSpikes();
    for(int i = 0; i<spikes.size(); i++){
        bool temp = Collision::intersectBool(m_outerHitbox, m_pos, spikes[i].m_aabb, spikes[i].m_pos, m_mini);
        if(temp){
            kill();
            return;
        }
    }

    std::vector<Portal>& portals = level->getPortals();
    for(int i = 0; i<portals.size(); i++){
        if(!portals[i].m_activated){
            bool temp = Collision::intersectBool(m_outerHitbox, m_pos, portals[i].m_aabb, portals[i].m_pos, m_mini);
            if(temp){
                portalInteraction(portals[i]);
            }
        }
    }

    std::vector<Orb>& orbs = level->getOrbs();
    for(int i = 0; i<orbs.size(); i++){
        if(!orbs[i].m_activated){
            bool temp = Collision::intersectBool(m_outerHitbox, m_pos, orbs[i].m_aabb, orbs[i].m_pos, m_mini);
            if(temp){
                if(orbInteraction(orbs[i])){
                    break;
                }
            }
        }
    }

    std::vector<Pad>& pads = level->getPads();
    for(int i = 0; i<pads.size(); i++){
        if(!pads[i].m_activated){
            bool temp = Collision::intersectBool(m_outerHitbox, m_pos, pads[i].m_aabb, pads[i].m_pos, m_mini);
            if(temp){
                padInteraction(pads[i]);
            }
        }
    }

    setGrounded(false);

    std::vector<Block> blocks = level->getBlocks();
    for(int i = 0; i<blocks.size(); i++){
        if(Collision::intersectBool(m_innerHitbox, m_pos, blocks[i].m_aabb, blocks[i].m_pos, m_mini)){
            kill();
            return;
        }

        CollisionData temp = Collision::intersectDiscrete(m_outerHitbox, m_pos, blocks[i].m_aabb, blocks[i].m_pos, m_mini);
        if(resolveBoxCollision(temp)){
            return;
        }
    }

    CollisionData tempGround = Collision::intersectDiscrete(m_outerHitbox, m_pos, {false, false, 1, -135, true}, m_mini);
    if(tempGround.m_collision){
        resolveCollision(tempGround);
        return;
    }
}

bool Player::resolveBoxCollision(CollisionData data){
    if(!data.m_collision){
        return false;
    }
    double dy = data.m_yCorrected - m_pos.y;
    if(!m_flippedGravity){
        if(dy > 0 && m_vel.y <= 0 && dy < 0.125){
            m_grounded = true;
            m_vel.y = 0;
            m_pos.y = data.m_yCorrected;
            return true;
        }
    }
    else{
        if(dy < 0 && m_vel.y >= 0 && dy > -0.125){
            m_grounded = true;
            m_vel.y = 0;
            m_pos.y = data.m_yCorrected;
            return true;
        }
    }
    return false;
}
