#pragma once

#include <glm/glm.hpp>

class AABB
{
public:
    AABB(glm::dvec2 minBound, glm::dvec2 maxBound, bool flipX, bool flipY);
    ~AABB();

    glm::dvec2 static intersect(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2);
    bool static intersectBool(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2);

    bool m_flipX = false;
    bool m_flipY = false;
private:
    glm::dvec2 m_min;
    glm::dvec2 m_max;
};
