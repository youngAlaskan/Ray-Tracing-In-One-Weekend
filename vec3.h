#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
    vec3() : m_e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : m_e{ e0, e1, e2 } {}

    double x() const { return m_e[0]; }
    double y() const { return m_e[1]; }
    double z() const { return m_e[2]; }

    vec3 operator-() const { return vec3(-m_e[0], -m_e[1], -m_e[2]); }
    double operator[](int i) const { return m_e[i]; }
    double& operator[](int i) { return m_e[i]; }

    vec3& operator+=(const vec3& v) {
        m_e[0] += v.m_e[0];
        m_e[1] += v.m_e[1];
        m_e[2] += v.m_e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        m_e[0] *= t;
        m_e[1] *= t;
        m_e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(lengthSquared());
    }

    double lengthSquared() const {
        return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
    }

public:
    double m_e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.m_e[0] << ' ' << v.m_e[1] << ' ' << v.m_e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.m_e[0] + v.m_e[0], u.m_e[1] + v.m_e[1], u.m_e[2] + v.m_e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.m_e[0] - v.m_e[0], u.m_e[1] - v.m_e[1], u.m_e[2] - v.m_e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.m_e[0] * v.m_e[0], u.m_e[1] * v.m_e[1], u.m_e[2] * v.m_e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.m_e[0], t * v.m_e[1], t * v.m_e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.m_e[0] * v.m_e[0]
        + u.m_e[1] * v.m_e[1]
        + u.m_e[2] * v.m_e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.m_e[1] * v.m_e[2] - u.m_e[2] * v.m_e[1],
        u.m_e[2] * v.m_e[0] - u.m_e[0] * v.m_e[2],
        u.m_e[0] * v.m_e[1] - u.m_e[1] * v.m_e[0]);
}

inline vec3 unitVector(vec3 v) {
    return v / v.length();
}