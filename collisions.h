#pragma once

#include <glm/glm.hpp>
#include <aabb.h>

struct CollisionData{
    bool m_collision = false;
    bool m_kill = false;
    double m_yCorrected = 0;
    double m_t;
};

class Collisions
{
public:
    bool static intersectBoolAABBAABB(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2);
    CollisionData static intersectDiscreteAABBAABB(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2);
    CollisionData static intersectContinuousAABBAABB(AABB aabb1, glm::dvec2 i1, glm::dvec2 d1, AABB aabb2, glm::dvec2 i2, glm::dvec2 d2);

    CollisionData static intersectDiscreteAABBXAP(AABB aabb, glm::dvec2 pos, double y, bool up);
};
