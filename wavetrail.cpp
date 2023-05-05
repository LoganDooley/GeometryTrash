#include "wavetrail.h"

#include "debug.h"

WaveTrail::WaveTrail(std::shared_ptr<Settings> settings, glm::dvec2 playerPos):
    m_settings(settings)
{
    m_data = {
        playerPos.x, playerPos.y-0.125,
        playerPos.x, playerPos.y-0.125,
        playerPos.x, playerPos.y+0.125,
        playerPos.x, playerPos.y+0.125,
        playerPos.x, playerPos.y+0.125,
        playerPos.x, playerPos.y-0.125
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(GLdouble), m_data.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_waveShader = ShaderLoader::createShaderProgram("Shaders/wave.vert", "Shaders/wave.frag");
    Debug::checkGLError();
}

WaveTrail::~WaveTrail(){
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void WaveTrail::addSegment(glm::dvec2 playerPos){
    update(playerPos);
    m_data.push_back(playerPos.x);
    m_data.push_back(playerPos.y-0.125);
    m_data.push_back(playerPos.x);
    m_data.push_back(playerPos.y-0.125);
    m_data.push_back(playerPos.x);
    m_data.push_back(playerPos.y+0.125);
    m_data.push_back(playerPos.x);
    m_data.push_back(playerPos.y+0.125);
    m_data.push_back(playerPos.x);
    m_data.push_back(playerPos.y+0.125);
    m_data.push_back(playerPos.x);
    m_data.push_back(playerPos.y-0.125);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(GLdouble), m_data.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void WaveTrail::update(glm::dvec2 playerPos){
    m_data[m_data.size() - 5] = playerPos.y+0.125;
    m_data[m_data.size() - 6] = playerPos.x;
    m_data[m_data.size() - 7] = playerPos.y+0.125;
    m_data[m_data.size() - 8] = playerPos.x;
    m_data[m_data.size() - 9] = playerPos.y-0.125;
    m_data[m_data.size() - 10] = playerPos.x;
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(GLdouble), m_data.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void WaveTrail::draw(glm::dvec2 playerPos, glm::vec3 waveColor){
    //std::cout<<"Drawing wave trail with "<<m_data.size()/2<<" vertices"<<std::endl;
    glUseProgram(m_waveShader);
    glUniform2f(glGetUniformLocation(m_waveShader, "playerPos"), float(playerPos.x), float(playerPos.y));
    glUniform2f(glGetUniformLocation(m_waveShader, "screenDim"), m_settings->m_width, m_settings->m_height);
    glUniform1f(glGetUniformLocation(m_waveShader, "yUnits"), m_settings->m_yUnits);
    glUniform3f(glGetUniformLocation(m_waveShader, "waveColor"), waveColor.r, waveColor.g, waveColor.b);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_data.size()/2);
    glBindVertexArray(0);
    glUseProgram(0);
}

void WaveTrail::prune(double screenLeft){
    bool pruned = false;
    for(int i = 0; i<m_data.size()/6; i+= 6){
        if(m_data.at(0) < screenLeft && m_data.at(2) < screenLeft){
            m_data.erase(m_data.begin(), m_data.begin()+12);
            pruned = true;
        }
        else{
            break;
        }
    }

    if(pruned){
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(GLdouble), m_data.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
}
