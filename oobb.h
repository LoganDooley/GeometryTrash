#pragma once

#include <glm/glm.hpp>

class OOBB
{
public:
    OOBB(glm::dvec2 minBound, glm::dvec2 maxBound, glm::dvec2 axis1, glm::dvec2 axis2);
    ~OOBB();

    void rotate(float dTheta);
    bool static intersectBool(OOBB oobb1, glm::dvec2 pos1, OOBB oobb2, glm::dvec2 pos2);
    glm::dvec2 support(glm::dvec2 d) const;

private:
    glm::dvec2 m_min;
    glm::dvec2 m_max;
    glm::dvec2 m_axis1;
    glm::dvec2 m_axis2;
};
