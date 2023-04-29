#include "aabb.h"

#include <iostream>

AABB::AABB(glm::dvec2 minBound, glm::dvec2 maxBound, bool flipX, bool flipY):
    m_min(minBound), m_max(maxBound), m_flipX(flipX), m_flipY(flipY)
{

}

AABB::~AABB(){

}

glm::dvec2 AABB::intersect(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2){
    glm::dvec2 min1 = aabb1.m_min + pos1;
    glm::dvec2 max1 = aabb1.m_max + pos1;
    glm::dvec2 min2 = aabb2.m_min + pos2;
    glm::dvec2 max2 = aabb2.m_max + pos2;
    if(aabb1.m_flipX){
        std::swap(min1.x, max1.x);
    }
    if(aabb1.m_flipY){
        std::swap(min1.y, max1.y);
    }
    if(aabb2.m_flipX){
        std::swap(min2.x, max2.x);
    }
    if(aabb2.m_flipY){
        std::swap(min2.y, max2.y);
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
        if(abs(max1.y - min2.y) < abs(max2.y - min1.y)){
            yOverlap = min2.y - max1.y;
        }
        else{
            yOverlap = max2.y - min1.y;
        }
    }

    //std::cout<<xOverlap<<" "<<yOverlap<<std::endl;

    if(yOverlap == 0 || xOverlap == 0){
        return glm::dvec2(0, 0);
    }
    else if(abs(yOverlap) > abs(xOverlap)){
        return glm::dvec2(0, yOverlap);
    }
    else{
        return glm::dvec2(xOverlap, 0);
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
