#pragma once

#include <glm/glm.hpp>

struct AABB
{
    AABB(glm::dvec2 minBound, glm::dvec2 maxBound, bool flipX, bool flipY);
    ~AABB();

    double static intersect(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2);
    glm::dvec2 static intersectContinuous(AABB aabb1, glm::dvec2 i1, glm::dvec2 d1, AABB aabb2, glm::dvec2 i2, glm::dvec2 d2);
    bool static intersectBool(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2);

    bool m_flipX = false;
    bool m_flipY = false;

    glm::dvec2 m_min;
    glm::dvec2 m_max;
};
