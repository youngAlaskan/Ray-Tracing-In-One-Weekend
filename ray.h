#pragma once

#include "vec3.h"

class ray {
public:
    ray() : m_time(0.0) {}
    ray(const point3& origin, const vec3& direction, double time = 0.0)
        : m_origin(origin), m_direction(direction), m_time(time) {}

    point3 getOrigin() const { return m_origin; }
    vec3 getDirection() const { return m_direction; }
    double getTime() const { return m_time; }

    point3 resize(double t) const {
        return m_origin + t * m_direction;
    }

private:
    point3 m_origin;
    vec3 m_direction;
    double m_time;
};