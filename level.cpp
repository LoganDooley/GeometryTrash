#include "level.h"
#include "debug.h"
#include "player.h"

Level::Level(std::shared_ptr<Settings> settings):
    m_settings(settings)
{
    GLdouble spikeData[6] = {
        -15, -15,
        15, -15,
        0, 15
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

    GLdouble slope1Data[6] = {
        -15, -15,
        15, -15,
        15, 15
    };

    glGenVertexArrays(1, &m_slope1Vao);
    glBindVertexArray(m_slope1Vao);
    glGenBuffers(1, &m_slope1Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_slope1Vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(slope1Data), slope1Data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLdouble slope2Data[6] = {
        -30, -15,
        30, -15,
        30, 15
    };

    glGenVertexArrays(1, &m_slope2Vao);
    glBindVertexArray(m_slope2Vao);
    glGenBuffers(1, &m_slope2Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_slope2Vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(slope2Data), slope2Data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLdouble blockData[12] = {
        -15, -15,
        15, -15,
        15, 15,
        15, 15,
        -15, 15,
        -15, -15
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
        -15, -45,
        15, -45,
        15, 45,
        15, 45,
        -15, 45,
        -15, -45
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
        -15, -15,
        15, -15,
        15, 15,
        15, 15,
        -15, 15,
        -15, -15
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

    GLdouble padData[12] = {
        -15, -15,
        15, -15,
        15, -8,
        15, -8,
        -15, -8,
        -15, -15
    };

    glGenVertexArrays(1, &m_padVao);
    glBindVertexArray(m_padVao);
    glGenBuffers(1, &m_padVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_padVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(padData), padData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_levelShader = ShaderLoader::createShaderProgram("Shaders/level.vert", "Shaders/level.frag");
    Debug::checkGLError();

    m_spikes.emplace_back(glm::dvec2(300, -120), false, false);
    m_spikes.emplace_back(glm::dvec2(330, -120), false, false);
    m_spikes.emplace_back(glm::dvec2(360, -120), false, false);

    m_portals.emplace_back(glm::dvec2(450, -90), PortalType::MiniSize, false, false);

    m_spikes.emplace_back(glm::dvec2(510, -120), false, false);
    m_spikes.emplace_back(glm::dvec2(540, -120), false, false);

    m_portals.emplace_back(glm::dvec2(630, -90), PortalType::Ship, false, false);

}

Level::~Level()
{

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
    glBindVertexArray(m_slope1Vao);
    for(int i = 0; i<m_slope1s.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_slope1s[i].m_pos.x, m_slope1s[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), 0.75, 0.75, 0.75);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_slope1s[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_slope1s[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(m_slope2Vao);
    for(int i = 0; i<m_slope2s.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_slope2s[i].m_pos.x, m_slope2s[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), 0.75, 0.75, 0.75);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_slope2s[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_slope2s[i].m_flipY);
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
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_orbs[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_orbs[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(m_padVao);
    for(int i = 0; i<m_pads.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_pads[i].m_pos.x, m_pads[i].m_pos.y);
        glUniform3f(glGetUniformLocation(m_levelShader, "objColor"), m_pads[i].color().x, m_pads[i].color().y, m_pads[i].color().z);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipX"), m_pads[i].m_flipX);
        glUniform1i(glGetUniformLocation(m_levelShader, "flipY"), m_pads[i].m_flipY);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}
