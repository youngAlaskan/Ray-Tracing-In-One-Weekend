#pragma once

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : m_origin(origin), m_direction(direction) {}

    point3 getOrigin() const { return m_origin; }
    vec3 getDirection() const { return m_direction; }

    point3 resize(double t) const {
        return m_origin + t * m_direction;
    }

public:
    point3 m_origin;
    vec3 m_direction;
};