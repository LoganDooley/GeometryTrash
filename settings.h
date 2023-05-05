#pragma once

#include <glm/glm.hpp>

struct Settings
{
    Settings(int width, int height, double yUnits, double time):
        m_width(width), m_height(height), m_yUnits(yUnits), m_time(time)
    {

    }
    int m_width;
    int m_height;
    double m_yUnits;
    double m_time;
};
