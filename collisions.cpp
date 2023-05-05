#include "collisions.h"

CollisionData Collisions::intersectDiscreteAABBAABB(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2){
    CollisionData result;
    glm::dvec2 m1 = aabb1.m_min + pos1;
    glm::dvec2 M1 = aabb1.m_max + pos1;
    glm::dvec2 m2 = aabb2.m_min + pos2;
    glm::dvec2 M2 = aabb2.m_max + pos2;

    double yOverlap = 0;
    if(M1.y > m2.y && M2.y > m1.y){
        if(abs(m2.y - M1.y) < abs(M2.y - m1.y)){
            yOverlap = m2.y - M1.y;
        }
        else{
            yOverlap = M2.y - m1.y;
        }
    }

    double xOverlap = 0;
    if(M1.x > m2.x && M2.x > m1.x){
        if(abs(m2.x - M1.x) < abs(M2.x - m1.x)){
            xOverlap = m2.x - M1.x;
        }
        else{
            xOverlap = M2.x - m1.x;
        }
    }

    if(xOverlap == 0 || yOverlap == 0){
        return result;
    }
    else if(abs(xOverlap) < abs(yOverlap) && abs(xOverlap) > 0.25){
        result.m_kill = true;
        result.m_collision = true;
        return result;
    }
    else{
        result.m_collision = true;
        result.m_yCorrected = pos1.y + yOverlap;
        return result;
    }
}

bool Collisions::intersectBoolAABBAABB(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2){
    glm::dvec2 min1 = aabb1.m_min + pos1;
    glm::dvec2 max1 = aabb1.m_max + pos1;
    glm::dvec2 min2 = aabb2.m_min + pos2;
    glm::dvec2 max2 = aabb2.m_max + pos2;

    if(!(max1.x > min2.x && max2.x > min1.x)){
        return false;
    }

    if(!(max1.y > min2.y && max2.y > min1.y)){
        return false;
    }

    return true;
}

CollisionData Collisions::intersectContinuousAABBAABB(AABB aabb1, glm::dvec2 i1, glm::dvec2 d1, AABB aabb2, glm::dvec2 i2, glm::dvec2 d2){
    CollisionData result;

    glm::dvec2 m1 = aabb1.m_min;
    glm::dvec2 M1 = aabb1.m_max;
    glm::dvec2 m2 = aabb2.m_min;
    glm::dvec2 M2 = aabb2.m_max;
    //X offset minimum t:
    double mxt = (M2.x + i2.x - (m1.x - i1.x))/(d1.x - d2.x);
    //X offset maximum t:
    double Mxt = (m2.x + i2.x - (M1.x - i1.x))/(d1.x - d2.x);
    //Y offset minimum t:
    double myt = (M2.y + i2.y - (m1.y - i1.y))/(d1.y - d2.y);
    //Y offset maximum t:
    double Myt = (m2.y + i2.y - (M1.y - i1.y))/(d1.y - d2.y);

    if(Mxt < 0 || Myt < 0 || mxt > 1 || myt > 1 || mxt > Myt || myt > Mxt){
        // No collision
        return result;
    }
    mxt = fmax(0, mxt);
    myt = fmax(0, myt);
    double t = fmin(mxt, myt);
    glm::dvec2 finalPos = i1 + d1 * t;
    result.m_collision = true;
    result.m_t = t;
    if(t == mxt){
        result.m_kill = true;
        return result;
    }
    result.m_yCorrected = finalPos.y;
    return result;
}

CollisionData Collisions::intersectDiscreteAABBXAP(AABB aabb, glm::dvec2 pos, double y, bool up){
    CollisionData result;

    double m = aabb.m_min.y + pos.y;
    double M = aabb.m_max.y + pos.y;

    if(up){
        double yOffset = y - m;
        if(yOffset <= 0){
            return result;
        }
        else{
            result.m_collision = true;
            result.m_yCorrected = pos.y + yOffset;
            return result;
        }
    }
    else{
        double yOffset = y - M;
        if(yOffset >= 0){
            return result;
        }
        else{
            result.m_collision = true;
            result.m_yCorrected = pos.y + yOffset;
            return result;
        }
    }
}
