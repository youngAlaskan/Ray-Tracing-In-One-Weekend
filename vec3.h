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

    inline static vec3 random() {
        return vec3(randomDouble(), randomDouble(), randomDouble());
    }

    inline static vec3 random(double min, double max) {
        return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
    }

    inline bool nearZero() const {
        const double epsilon = 1e-8;
        return (abs(m_e[0]) < epsilon) && (abs(m_e[1]) < epsilon) && (abs(m_e[2]) < epsilon);
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

inline vec3 reflect(const vec3& vector, const vec3& normal) {
    return vector - 2 * dot(vector, normal) * normal;
}

inline vec3 refract(const vec3& uv, const vec3& normal, double coeff) {
    double cosTheta = fmin(dot(-uv, normal), 1.0);
    vec3 perpR = coeff * (uv + cosTheta * normal);
    vec3 parallelR = -sqrt(fabs(1.0 - perpR.lengthSquared())) * normal;
    return perpR + parallelR;
}

inline vec3 refract(const vec3& uv, const vec3& normal, double cosTheta, double coeff) {
    vec3 perpR = coeff * (uv + cosTheta * normal);
    vec3 parallelR = -sqrt(fabs(1.0 - perpR.lengthSquared())) * normal;
    return perpR + parallelR;
}

inline vec3 randomInUnitSphere() {
    while (true) {
        vec3 point = vec3::random(-1.0, 1.0);
        if (point.lengthSquared() >= 1.0) continue;
        return point;
    }
}

inline vec3 randomUnitVector() {
    return unitVector(randomInUnitSphere());
}

inline vec3 randomInHemisphere(const vec3& normal) {
    // Alternate diffuse method
    vec3 inUnitSphere = randomInUnitSphere();
    return dot(inUnitSphere, normal) > 0.0 ? inUnitSphere : -inUnitSphere;
}
