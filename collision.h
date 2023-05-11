#pragma once

#include "colliders.h"

struct CollisionData{
    bool m_collision = false;
    bool m_kill = false;
    double m_yCorrected = 0;
    double m_slope = 0;
};

struct Interval{
    double m_min;
    double m_max;
};

class Collision
{
public:
    bool static intersectBool(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2, bool mini);
    CollisionData static intersectDiscrete(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2, bool mini);
    CollisionData static intersectDiscrete(AABB aabb, glm::dvec2 pos, XAlignedPlane plane, bool mini);
    CollisionData static intersectDiscrete(AABB aabb, Circle circle, glm::dvec2 circlePos, Slope45 slope, glm::dvec2 slopePos, bool mini);
    CollisionData static intersectDiscrete(AABB aabb, Circle circle, glm::dvec2 circlePos, Slope60 slope, glm::dvec2 slopePos, bool mini);

    Interval static getInterval(Circle circle, glm::dvec2 circlePos, glm::dvec2 axis);
    Interval static getInterval(Slope45, glm::dvec2 slopePos, glm::dvec2 axis);
    Interval static getInterval(Slope60, glm::dvec2 slopePos, glm::dvec2 axis);

    double static intervalMTV(Interval a, Interval b);
};
