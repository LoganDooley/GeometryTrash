#pragma once

#include <glm/glm.hpp>

struct Collider
{
    bool m_flipX;
    bool m_flipY;
    double m_scale = 1;
};

struct AABB : public Collider
{
    glm::dvec2 m_min;
    glm::dvec2 m_max;
};

struct Circle : public Collider
{

};


struct Slope45 : public Collider
{
    glm::dvec2 getClosestPoint(glm::dvec2 pos, glm::dvec2 point){
        glm::dvec2 tempPoint = glm::dvec2(0.5, 0.5) * m_scale;
        if(m_flipX){
            tempPoint.y *= -1;
        }
        if(m_flipY){
            tempPoint.x *= -1;
        }
        double min = glm::length(point - pos + tempPoint);
        glm::dvec2 closestPoint = tempPoint;

        tempPoint = glm::dvec2(0.5, -0.5) * m_scale;
        if(m_flipX){
            tempPoint.y *= -1;
        }
        if(m_flipY){
            tempPoint.x *= -1;
        }
        double temp = glm::length(point - pos + tempPoint);
        if(temp < min){
            min = temp;
            closestPoint = tempPoint;
        }

        tempPoint = glm::dvec2(-0.5, -0.5) * m_scale;
        if(m_flipX){
            tempPoint.y *= -1;
        }
        if(m_flipY){
            tempPoint.x *= -1;
        }
        temp = glm::length(point - pos + tempPoint);
        if(temp < min){
            min = temp;
            closestPoint = tempPoint;
        }

        return closestPoint;
    }

    glm::dvec2 getCornerPoint(glm::dvec2 pos){
        glm::dvec2 cornerPoint = glm::dvec2(0.5, -0.5);
        if(m_flipX){
            cornerPoint.y *= -1;
        }
        if(m_flipY){
            cornerPoint.x *= -1;
        }
        return cornerPoint + pos;
    }
};

struct Slope60 : public Collider
{
    glm::dvec2 getClosestPoint(glm::dvec2 pos, glm::dvec2 point){
        glm::dvec2 tempPoint = glm::dvec2(1, 0.5) * m_scale;
        if(m_flipX){
            tempPoint.y *= -1;
        }
        if(m_flipY){
            tempPoint.x *= -1;
        }
        double min = glm::length(point - pos + tempPoint);
        glm::dvec2 closestPoint = tempPoint;

        tempPoint = glm::dvec2(1, -0.5) * m_scale;
        if(m_flipX){
            tempPoint.y *= -1;
        }
        if(m_flipY){
            tempPoint.x *= -1;
        }
        double temp = glm::length(point - pos + tempPoint);
        if(temp < min){
            min = temp;
            closestPoint = tempPoint;
        }

        tempPoint = glm::dvec2(-1, -0.5) * m_scale;
        if(m_flipX){
            tempPoint.y *= -1;
        }
        if(m_flipY){
            tempPoint.x *= -1;
        }
        temp = glm::length(point - pos + tempPoint);
        if(temp < min){
            min = temp;
            closestPoint = tempPoint;
        }

        return closestPoint;
    }
};

struct XAlignedPlane : public Collider
{
    double m_y;
    bool m_up;
};
