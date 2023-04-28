#include "level.h"
#include "debug.h"

Level::Level():
    m_spikeHitbox(AABB(glm::dvec2(-0.1, -0.1), glm::dvec2(0.1, 0.3)))
{
    GLdouble data[6] = {
        -0.5, -0.5,
        0.5, -0.5,
        0, 0.5,
    };

    glGenVertexArrays(1, &m_spikeVao);
    glBindVertexArray(m_spikeVao);
    glGenBuffers(1, &m_spikeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_spikeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_levelShader = ShaderLoader::createShaderProgram("Shaders/level.vert", "Shaders/level.frag");
    Debug::checkGLError();

    float startPos = 5;
    for(int i = 0; i<10; i++){
        m_spikes.push_back({glm::dvec2(startPos, -4)});
        m_spikes.push_back({glm::dvec2(startPos+1, -4)});
        m_spikes.push_back({glm::dvec2(startPos+2, -4)});
        m_spikes.push_back({glm::dvec2(startPos+3.25, -2)});
        startPos += 4.5;
    }
}

Level::~Level()
{

}

void Level::checkCollisions(std::shared_ptr<Player> player){
    glm::dvec2 collision = glm::dvec2(0, 0);
    for(int i = 0; i<m_spikes.size(); i++){
        glm::dvec2 temp = AABB::intersect(m_spikeHitbox, m_spikes[i].m_pos, player->getHitbox(), player->getPos());
        if(temp != glm::dvec2(0, 0)){
            collision = temp;
        }
    }
    player->resolveCollision(collision);
}

void Level::draw(std::shared_ptr<Player> player, glm::vec2 screenDim){
    glUseProgram(m_levelShader);
    glUniform2f(glGetUniformLocation(m_levelShader, "playerPos"), player->getPos().x, player->getPos().y);
    glUniform2f(glGetUniformLocation(m_levelShader, "screenDim"), screenDim.x, screenDim.y);
    glBindVertexArray(m_spikeVao);
    for(int i = 0; i<m_spikes.size(); i++){
        glUniform2f(glGetUniformLocation(m_levelShader, "objPos"), m_spikes[i].m_pos.x, m_spikes[i].m_pos.y);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}
