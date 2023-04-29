#include "level.h"
#include "debug.h"

Level::Level()
{
    GLdouble spikeData[6] = {
        -0.5, -0.5,
        0.5, -0.5,
        0, 0.5,
    };

    glGenVertexArrays(1, &m_spikeVao);
    glBindVertexArray(m_spikeVao);
    glGenBuffers(1, &m_spikeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_spikeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spikeData), spikeData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLdouble portalData[12] = {
        -0.5, -1.5,
        0.5, -1.5,
        0.5, 1.5,
        0.5, 1.5,
        -0.5, 1.5,
        -0.5, -1.5
    };

    glGenVertexArrays(1, &m_portalVao);
    glBindVertexArray(m_portalVao);
    glGenBuffers(1, &m_portalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_portalVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(portalData), portalData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLdouble orbData[12] = {
        -0.5, -0.5,
        0.5, -0.5,
        0.5, 0.5,
        0.5, 0.5,
        -0.5, 0.5,
        -0.5, -0.5
    };

    glGenVertexArrays(1, &m_orbVao);
    glBindVertexArray(m_orbVao);
    glGenBuffers(1, &m_orbVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_orbVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(orbData), orbData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_levelShader = ShaderLoader::createShaderProgram("Shaders/level.vert", "Shaders/level.frag");
    Debug::checkGLError();

    float currPos = 5;
    m_spikes.emplace_back(glm::dvec2(currPos, -2), false, true);
    m_spikes.emplace_back(glm::dvec2(currPos+1, -4), false, false);
    m_spikes.emplace_back(glm::dvec2(currPos+2, -4), false, false);
    m_spikes.emplace_back(glm::dvec2(currPos+3, -4), false, false);
    m_spikes.emplace_back(glm::dvec2(currPos+4, -2), false, true);
    currPos += 10;

    m_orbs.emplace_back(glm::dvec2(currPos, -2), OrbType::Blue, false, false);

    currPos += 3;

    m_spikes.emplace_back(glm::dvec2(currPos, 2), false, false);
    m_spikes.emplace_back(glm::dvec2(currPos+1, 4), false, true);
    m_spikes.emplace_back(glm::dvec2(currPos+2, 4), false, true);
    m_spikes.emplace_back(glm::dvec2(currPos+3, 4), false, true);
    currPos += 10;

    m_orbs.emplace_back(glm::dvec2(currPos, 2), OrbType::Red, false, false);

    currPos += 3;

    m_orbs.emplace_back(glm::dvec2(currPos, -2), OrbType::Green, false, false);

    currPos += 3;

    m_portals.emplace_back(glm::dvec2(currPos, 0), PortalType::Ship, false, false);

    currPos += 5;
    for(int i = 0; i<5; i++){
        m_spikes.emplace_back(glm::dvec2(currPos, -1), false, false);
        m_spikes.emplace_back(glm::dvec2(currPos, 1), false, true);
        currPos += 1;
    }

    currPos += 3;
    for(int i = 0; i<5; i++){
        m_spikes.emplace_back(glm::dvec2(currPos, -2), false, false);
        m_spikes.emplace_back(glm::dvec2(currPos, 0), false, true);
        currPos += 1;
    }

    currPos += 8;
    for(int i = 0; i<5; i++){
        m_spikes.emplace_back(glm::dvec2(currPos, 1), false, false);
        m_spikes.emplace_back(glm::dvec2(currPos, 3), false, true);
        currPos += 1;
    }
}

Level::~Level()
{

}

void Level::checkCollisions(std::shared_ptr<Player> player){
    glm::dvec2 collision = glm::dvec2(0, 0);
    for(int i = 0; i<m_spikes.size(); i++){
        glm::dvec2 temp = AABB::intersect(m_spikes[i].m_aabb, m_spikes[i].m_pos, player->getHitbox(), player->getPos());
        if(temp != glm::dvec2(0, 0)){
            collision = temp;
        }
    }
    player->resolveCollision(collision);

    for(int i = 0; i<m_portals.size(); i++){
        glm::dvec2 temp = AABB::intersect(m_portals[i].m_aabb, m_portals[i].m_pos, player->getHitbox(), player->getPos());
        if(temp != glm::dvec2(0, 0)){
            player->portalInteraction(m_portals[i]);
        }
    }

    for(int i = 0; i<m_orbs.size(); i++){
        bool temp = AABB::intersectBool(m_orbs[i].m_aabb, m_orbs[i].m_pos, player->getHitbox(), player->getPos());
        if(temp){
            if(player->orbInteraction(m_orbs[i])){
                break;
            }
        }
    }
}

void Level::draw(std::shared_ptr<Player> player, glm::vec2 screenDim){
    glUseProgram(m_levelShader);
    glUniform2f(glGetUniformLocation(m_levelShader, "playerPos"), player->getPos().x, player->getPos().y);
    glUniform2f(glGetUniformLocation(m_levelShader, "screenDim"), screenDim.x, screenDim.y);
    glBindVertexArray(m_spikeVao);
    for(int i = 0; i<m_spikes.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_spikes[i].m_pos.x, m_spikes[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), 1, 1, 1);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_spikes[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_spikes[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(m_portalVao);
    for(int i = 0; i<m_portals.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_portals[i].m_pos.x, m_portals[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), m_portals[i].color().x, m_portals[i].color().y, m_portals[i].color().z);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_portals[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_portals[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(m_orbVao);
    for(int i = 0; i<m_orbs.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_orbs[i].m_pos.x, m_orbs[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), m_orbs[i].color().x, m_orbs[i].color().y, m_orbs[i].color().z);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_portals[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_portals[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}
