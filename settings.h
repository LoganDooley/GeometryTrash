#pragma once

#include <glm/glm.hpp>

struct Settings
{
    Settings(int width, int height, double yUnits, double dtMultiplier = 10):
        m_width(width), m_height(height), m_yUnits(yUnits), m_dtMultiplier(dtMultiplier)
    {

    }
    int m_width;
    int m_height;
    double m_yUnits;
    double m_dtMultiplier;
};
