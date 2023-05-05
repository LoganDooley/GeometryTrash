#include "aabb.h"

#include <iostream>

AABB::AABB(glm::dvec2 minBound, glm::dvec2 maxBound, bool flipX, bool flipY):
    m_min(minBound), m_max(maxBound), m_flipX(flipX), m_flipY(flipY)
{

}

AABB::~AABB(){

}

double AABB::intersect(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2){
    glm::dvec2 min1 = aabb1.m_min + pos1;
    glm::dvec2 max1 = aabb1.m_max + pos1;
    glm::dvec2 min2 = aabb2.m_min + pos2;
    glm::dvec2 max2 = aabb2.m_max + pos2;
    if(aabb1.m_flipX){
        std::swap(min1.y, max1.y);
    }
    if(aabb1.m_flipY){
        std::swap(min1.x, max1.x);
    }
    if(aabb2.m_flipX){
        std::swap(min2.y, max2.y);
    }
    if(aabb2.m_flipY){
        std::swap(min2.x, max2.x);
    }

    float xOverlap = 0;
    if(max1.x > min2.x && max2.x > min1.x){
        if(abs(max1.x - min2.x) < abs(max2.x - min1.x)){
            xOverlap = min2.x - max1.x;
        }
        else{
            xOverlap = max2.x - min1.x;
        }
    }

    float yOverlap = 0;
    if(max1.y > min2.y && max2.y > min1.y){
        if(abs(min2.y - max1.y) < abs(max2.y - min1.y)){
            yOverlap = min2.y - max1.y;
        }
        else{
            yOverlap = max2.y - min1.y;
        }
    }

    if(yOverlap == 0 || xOverlap == 0){
        return 0;
    }
    else if(abs(yOverlap) > abs(xOverlap) && xOverlap > 0.5){
        return -100;
    }
    else{
        return yOverlap;
    }
}

bool AABB::intersectBool(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2){
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

glm::dvec2 AABB::intersectContinuous(AABB aabb1, glm::dvec2 i1, glm::dvec2 d1, AABB aabb2, glm::dvec2 i2, glm::dvec2 d2){
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
        return glm::dvec2(0, 0);
    }
    mxt = fmax(0, mxt);
    myt = fmax(0, myt);
    double t = fmin(mxt, myt);
    glm::dvec2 finalPos = i1 + d1 * t;
    return finalPos;
}
