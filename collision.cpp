#include "collision.h"

#include <vector>

bool Collision::intersectBool(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2, bool mini){
    glm::dvec2 min1 = aabb1.m_min/double(1 + mini) + pos1;
    glm::dvec2 max1 = aabb1.m_max/double(1 + mini) + pos1;
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

CollisionData Collision::intersectDiscrete(AABB aabb1, glm::dvec2 pos1, AABB aabb2, glm::dvec2 pos2, bool mini){
    CollisionData result;
    glm::dvec2 m1 = aabb1.m_min/double(1 + mini) + pos1;
    glm::dvec2 M1 = aabb1.m_max/double(1 + mini) + pos1;
    glm::dvec2 m2 = aabb2.m_min + pos2;
    glm::dvec2 M2 = aabb2.m_max + pos2;

    if(M1.x <= m2.x || M2.x <= m1.x){
        return result;
    }

    double yOverlap = 0;
    if(M1.y > m2.y && M2.y > m1.y){
        if(abs(m2.y - M1.y) < abs(M2.y - m1.y)){
            yOverlap = m2.y - M1.y;
        }
        else{
            yOverlap = M2.y - m1.y;
        }
    }

    if(yOverlap == 0){
        return result;
    }
    else{
        result.m_collision = true;
        result.m_yCorrected = pos1.y + yOverlap;
        result.m_slope = 0;
        return result;
    }
}

CollisionData Collision::intersectDiscrete(AABB aabb, glm::dvec2 pos, XAlignedPlane plane, bool mini){
    CollisionData result;

    double m = aabb.m_min.y/double(1 + mini) + pos.y;
    double M = aabb.m_max.y/double(1 + mini) + pos.y;

    if(plane.m_up){
        double yOffset = plane.m_y - m;
        if(yOffset > 0){
            result.m_collision = true;
            result.m_yCorrected = pos.y + yOffset;
        }
    }
    else{
        double yOffset = plane.m_y - M;
        if(yOffset < 0){
            result.m_collision = true;
            result.m_yCorrected = pos.y + yOffset;
        }
    }

    return result;
}

CollisionData Collision::intersectDiscrete(AABB aabb, Circle circle, glm::dvec2 circlePos, Slope45 slope, glm::dvec2 slopePos, bool mini){
    CollisionData result;

    glm::dvec2 slopePoint = glm::dvec2(0.5, 0.5) * slope.m_scale;
    if(slope.m_flipX){
        slopePoint.y *= -1;
    }
    if(slope.m_flipY){
        slopePoint.x *= -1;
    }
    slopePoint += slopePos;

    glm::dvec2 slopeAxis = glm::normalize(glm::dvec2(-1, 1));
    if(slope.m_flipX){
        slopeAxis.y *= -1;
    }
    if(slope.m_flipY){
        slopeAxis.x *= -1;
    }

    double difference = glm::dot(circlePos, slopeAxis) - glm::dot(slopePoint, slopeAxis);
    if(difference <= 0){
        return intersectDiscrete(aabb, circlePos, {slope.m_flipX, slope.m_flipY, slope.m_scale, glm::dvec2(-0.5, -0.5), glm::dvec2(0.5, 0.5)}, slopePos, mini);
    }
    if(difference > 0.5 * circle.m_scale){
        return result;
    }
    if(circlePos.x + 0.5 * circle.m_scale < slopePos.x - 0.5 * slope.m_scale || circlePos.x - 0.5 * circle.m_scale > slopePos.x + 0.5 * slope.m_scale){
        return result;
    }
    glm::dvec2 mtv = (0.5 * circle.m_scale - difference) * slopeAxis;

    double dy = mtv.y + pow(mtv.x, 2)/mtv.y;

    result.m_collision = true;
    result.m_slope = -slopeAxis.x/slopeAxis.y;
    result.m_yCorrected = circlePos.y + dy;

    return result;
}

CollisionData Collision::intersectDiscrete(AABB aabb, Circle circle, glm::dvec2 circlePos, Slope60 slope, glm::dvec2 slopePos, bool mini){
    CollisionData result;

    glm::dvec2 slopePoint = glm::dvec2(1, 0.5) * slope.m_scale;
    if(slope.m_flipX){
        slopePoint.y *= -1;
    }
    if(slope.m_flipY){
        slopePoint.x *= -1;
    }
    slopePoint += slopePos;

    glm::dvec2 slopeAxis = glm::normalize(glm::dvec2(-1, 2));
    if(slope.m_flipX){
        slopeAxis.y *= -1;
    }
    if(slope.m_flipY){
        slopeAxis.x *= -1;
    }

    double difference = glm::dot(circlePos, slopeAxis) - glm::dot(slopePoint, slopeAxis);
    if(difference <= 0){
        return intersectDiscrete(aabb, circlePos, {slope.m_flipX, slope.m_flipY, slope.m_scale, glm::dvec2(-1, -0.5), glm::dvec2(1, 0.5)}, slopePos, mini);
    }
    if(difference > 0.5 * circle.m_scale){
        return result;
    }
    if(circlePos.x + 0.5 * circle.m_scale < slopePos.x - 1 * slope.m_scale || circlePos.x - 0.5 * circle.m_scale > slopePos.x + 1 * slope.m_scale){
        return result;
    }
    glm::dvec2 mtv = (0.5 * circle.m_scale - difference) * slopeAxis;

    double dy = mtv.y + pow(mtv.x, 2)/mtv.y;

    result.m_collision = true;
    result.m_slope = -slopeAxis.x/slopeAxis.y;
    result.m_yCorrected = circlePos.y + dy;

    return result;
}

Interval static getInterval(Circle circle, glm::dvec2 circlePos, glm::dvec2 axis){
    Interval result;
    result.m_min = glm::dot(circlePos - 0.5 * circle.m_scale * axis, axis);
    result.m_max = glm::dot(circlePos + 0.5 * circle.m_scale * axis, axis);
    return result;
}

Interval static getInterval(Slope45 slope, glm::dvec2 slopePos, glm::dvec2 axis){
    Interval result;
    glm::dvec2 tempPoint = glm::dvec2(0.5, 0.5) * slope.m_scale;
    if(slope.m_flipX){
        tempPoint.y *= -1;
    }
    if(slope.m_flipY){
        tempPoint.x *= -1;
    }
    double temp = glm::dot(slopePos + tempPoint, axis);
    result.m_min = temp;
    result.m_max = temp;

    tempPoint = glm::dvec2(0.5, -0.5) * slope.m_scale;
    if(slope.m_flipX){
        tempPoint.y *= -1;
    }
    if(slope.m_flipY){
        tempPoint.x *= -1;
    }
    temp = glm::dot(slopePos + tempPoint, axis);
    result.m_min = fmin(result.m_min, temp);
    result.m_max = fmax(result.m_max, temp);

    tempPoint = glm::dvec2(-0.5, -0.5) * slope.m_scale;
    if(slope.m_flipX){
        tempPoint.y *= -1;
    }
    if(slope.m_flipY){
        tempPoint.x *= -1;
    }
    temp = glm::dot(slopePos + tempPoint, axis);
    result.m_min = fmin(result.m_min, temp);
    result.m_max = fmax(result.m_max, temp);

    return result;
}

Interval static getInterval(Slope60 slope, glm::dvec2 slopePos, glm::dvec2 axis){
    Interval result;
    glm::dvec2 tempPoint = glm::dvec2(1, 0.5) * slope.m_scale;
    if(slope.m_flipX){
        tempPoint.y *= -1;
    }
    if(slope.m_flipY){
        tempPoint.x *= -1;
    }
    double temp = glm::dot(slopePos + tempPoint, axis);
    result.m_min = temp;
    result.m_max = temp;

    tempPoint = glm::dvec2(1, -0.5) * slope.m_scale;
    if(slope.m_flipX){
        tempPoint.y *= -1;
    }
    if(slope.m_flipY){
        tempPoint.x *= -1;
    }
    temp = glm::dot(slopePos + tempPoint, axis);
    result.m_min = fmin(result.m_min, temp);
    result.m_max = fmax(result.m_max, temp);

    tempPoint = glm::dvec2(-1, -0.5) * slope.m_scale;
    if(slope.m_flipX){
        tempPoint.y *= -1;
    }
    if(slope.m_flipY){
        tempPoint.x *= -1;
    }
    temp = glm::dot(slopePos + tempPoint, axis);
    result.m_min = fmin(result.m_min, temp);
    result.m_max = fmax(result.m_max, temp);

    return result;
}

double static intervalMTV(Interval a, Interval b){
    double aRight = b.m_max - a.m_min;
    double aLeft = a.m_max - b.m_min;
    if(aLeft < 0 || aRight < 0){
        return 0;
    }
    else if(aRight < aLeft){
        return aRight;
    }
    else{
        return -aLeft;
    }
}
