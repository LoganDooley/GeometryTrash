#include "level.h"
#include "debug.h"

Level::Level(std::shared_ptr<Settings> settings):
    m_settings(settings)
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

    GLdouble blockData[12] = {
        -0.5, -0.5,
        0.5, -0.5,
        0.5, 0.5,
        0.5, 0.5,
        -0.5, 0.5,
        -0.5, -0.5
    };

    glGenVertexArrays(1, &m_blockVao);
    glBindVertexArray(m_blockVao);
    glGenBuffers(1, &m_blockVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_blockVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blockData), blockData, GL_STATIC_DRAW);
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

    //m_portals.emplace_back(glm::dvec2(5, -3), PortalType::Wave, false, false);
    m_spikes.emplace_back(glm::dvec2(8, -4), false, false);
    m_spikes.emplace_back(glm::dvec2(9, -4), false, false);
    m_blocks.emplace_back(glm::dvec2(10, -4), false, false);
}

Level::~Level()
{

}

void Level::checkCollisions(std::shared_ptr<Player> player){
    double collision = 0;
    for(int i = 0; i<m_spikes.size(); i++){
        bool temp = Collisions::intersectBoolAABBAABB(m_spikes[i].m_aabb, m_spikes[i].m_pos, player->getHitbox(), player->getPos());
        if(temp){
            player->kill();
        }
    }

    for(int i = 0; i<m_portals.size(); i++){
        bool temp = Collisions::intersectBoolAABBAABB(m_portals[i].m_aabb, m_portals[i].m_pos, player->getHitbox(), player->getPos());
        if(temp){
            player->portalInteraction(m_portals[i]);
        }
    }

    for(int i = 0; i<m_orbs.size(); i++){
        bool temp = Collisions::intersectBoolAABBAABB(m_orbs[i].m_aabb, m_orbs[i].m_pos, player->getHitbox(), player->getPos());
        if(temp){
            if(player->orbInteraction(m_orbs[i])){
                break;
            }
        }
    }

    player->setGrounded(false);

    for(int i = 0; i<m_blocks.size(); i++){
        CollisionData temp = Collisions::intersectDiscreteAABBAABB(player->getHitbox(), player->getPos(), m_blocks[i].m_aabb, m_blocks[i].m_pos);
        if(temp.m_kill){
            std::cout<<"Block death"<<std::endl;
            player->kill();
        }
        else if(temp.m_collision){
            player->resolveCollision(temp.m_yCorrected);
        }
    }

    CollisionData tempGround = Collisions::intersectDiscreteAABBXAP(player->getHitbox(), player->getPos(), -4.5, true);
    if(tempGround.m_collision){
        player->resolveCollision(tempGround.m_yCorrected);
    }
}

void Level::draw(std::shared_ptr<Player> player){
    glUseProgram(m_levelShader);
    glUniform2f(glGetUniformLocation(m_levelShader, "playerPos"), player->getPos().x, player->getPos().y);
    glUniform2f(glGetUniformLocation(m_levelShader, "screenDim"), m_settings->m_width, m_settings->m_height);
    glUniform1f(glGetUniformLocation(m_levelShader, "yUnits"), m_settings->m_yUnits);
    glBindVertexArray(m_spikeVao);
    for(int i = 0; i<m_spikes.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_spikes[i].m_pos.x, m_spikes[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), 1, 1, 1);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_spikes[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_spikes[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(m_blockVao);
    for(int i = 0; i<m_blocks.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_blocks[i].m_pos.x, m_blocks[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), 0.75, 0.75, 0.75);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_blocks[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_blocks[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 6);
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
