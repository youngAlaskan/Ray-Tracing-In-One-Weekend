#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class vec2 {
public:
    vec2() : m_e{0.0, 0.0} {}
    vec2(double e0, double e1) : m_e{ e0, e1 } {}

    double x() const { return m_e[0]; }
    double y() const { return m_e[1]; }

    vec2 operator-() const { return vec2(-m_e[0], -m_e[1]); }
    double operator[](int i) const { return m_e[i]; }
    double& operator[](int i) { return m_e[i]; }

    vec2& operator+=(const vec2& v) {
        m_e[0] += v.m_e[0];
        m_e[1] += v.m_e[1];
        return *this;
    }

    vec2& operator*=(const double t) {
        m_e[0] *= t;
        m_e[1] *= t;
        return *this;
    }

    vec2& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(lengthSquared());
    }

    double lengthSquared() const {
        return m_e[0] * m_e[0] + m_e[1] * m_e[1];
    }

    inline static vec2 random() {
        return vec2(randomDouble(), randomDouble());
    }

    inline static vec2 random(double min, double max) {
        return vec2(randomDouble(min, max), randomDouble(min, max));
    }

    inline bool nearZero() const {
        const double epsilon = 1e-8;
        return (abs(m_e[0]) < epsilon) && (abs(m_e[1]) < epsilon);
    }

public:
    double m_e[2];
};

// Type aliases for vec2
using point2 = vec2;   // 2D point

// vec2 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec2& v) {
    return out << v.m_e[0] << ' ' << v.m_e[1] << ' ' << v.m_e[2];
}

inline vec2 operator+(const vec2& u, const vec2& v) {
    return vec2(u.m_e[0] + v.m_e[0], u.m_e[1] + v.m_e[1]);
}

inline vec2 operator-(const vec2& u, const vec2& v) {
    return vec2(u.m_e[0] - v.m_e[0], u.m_e[1] - v.m_e[1]);
}

inline vec2 operator*(const vec2& u, const vec2& v) {
    return vec2(u.m_e[0] * v.m_e[0], u.m_e[1] * v.m_e[1]);
}

inline vec2 operator*(double t, const vec2& v) {
    return vec2(t * v.m_e[0], t * v.m_e[1]);
}

inline vec2 operator*(const vec2& v, double t) {
    return t * v;
}

inline vec2 operator/(vec2 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec2& u, const vec2& v) {
    return u.m_e[0] * v.m_e[0] + u.m_e[1] * v.m_e[1];
}

inline double cross(const vec2& u, const vec2& v) {
    return u.m_e[0] * v.m_e[1] - u.m_e[1] * v.m_e[0];
}

inline vec2 unitVector(vec2 v) {
    return v / v.length();
}
