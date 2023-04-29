#include "oobb.h"

OOBB::OOBB(glm::dvec2 minBound, glm::dvec2 maxBound, glm::dvec2 axis1, glm::dvec2 axis2):
    m_min(minBound),
    m_max(maxBound),
    m_axis1(glm::normalize(axis1)),
    m_axis2(glm::normalize(axis2))
{

}

OOBB::~OOBB(){

}

void OOBB::rotate(float dTheta){
    m_axis1 = glm::dvec2(glm::dot(glm::dvec2(cosf(dTheta), sinf(dTheta)), m_axis1), glm::dot(glm::dvec2(-sinf(dTheta), cosf(dTheta)), m_axis1));
    m_axis2 = glm::dvec2(glm::dot(glm::dvec2(cosf(dTheta), sinf(dTheta)), m_axis2), glm::dot(glm::dvec2(-sinf(dTheta), cosf(dTheta)), m_axis2));
}

bool OOBB::intersectBool(OOBB oobb1, glm::dvec2 pos1, OOBB oobb2, glm::dvec2 pos2){
    // oobb1 axis 1
    if((oobb1.m_axis1 * oobb1.m_max.x - oobb2.support(-oobb1.m_axis1) + pos1 - pos2).length() <= 0 || (oobb2.support(oobb1.m_axis1) - oobb1.m_axis1 * oobb1.m_min.x + pos1 - pos2).length() <= 0){
        return false;
    }

    // oobb1 axis 2
    if((oobb1.m_axis2 * oobb1.m_max.y - oobb2.support(-oobb1.m_axis2) + pos1 - pos2).length() <= 0 || (oobb2.support(oobb1.m_axis2) - oobb1.m_axis2 * oobb1.m_min.y + pos1 - pos2).length() <= 0){
        return false;
    }

    // oobb2 axis 1
    if((oobb2.m_axis1 * oobb2.m_max.x - oobb1.support(-oobb2.m_axis1) + pos2 - pos1).length() <= 0 || (oobb1.support(oobb2.m_axis1) - oobb2.m_axis1 * oobb2.m_min.x + pos2 - pos1).length() <= 0){
        return false;
    }

    // oobb2 axis 2
    if((oobb2.m_axis2 * oobb2.m_max.y - oobb1.support(-oobb2.m_axis2) + pos2 - pos1).length() <= 0 || (oobb1.support(oobb2.m_axis2) - oobb2.m_axis2 * oobb2.m_min.y + pos2 - pos1).length() <= 0){
        return false;
    }

    return true;
}

glm::dvec2 OOBB::support(glm::dvec2 d) const{
    glm::dvec2 point = m_axis1 * m_min.x;
    float dist = glm::dot(d, point);

    glm::dvec2 tempPoint = m_axis1 * m_max.x;
    float tempDist = glm::dot(d, tempPoint);
    if(tempDist > dist){
        dist = tempDist;
        point = tempPoint;
    }

    tempPoint = m_axis2 * m_min.y;
    tempDist = glm::dot(d, tempPoint);
    if(tempDist > dist){
        dist = tempDist;
        point = tempPoint;
    }

    tempPoint = m_axis2 * m_max.y;
    tempDist = glm::dot(d, tempPoint);
    if(tempDist > dist){
        point = tempPoint;
    }

    return point;
}
