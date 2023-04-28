#pragma once

#include <glm/glm.hpp>

class AABB
{
public:
    AABB(glm::dvec2 minBound, glm::dvec2 maxBound);;
    ~AABB();

    glm::dvec2 static intersect(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2);

private:
    glm::dvec2 m_min;
    glm::dvec2 m_max;
};
