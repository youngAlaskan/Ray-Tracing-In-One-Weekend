#pragma once

#include "hittable.h"

class sphere : public hittable {
public:
    sphere() : m_center(vec3(0.0, 0.0, 0.0)), m_radius(0.0) {}
	sphere(point3 center, double r, shared_ptr<material> material_ptr)
        : m_center(center), m_radius(r), m_material_ptr(material_ptr) {};

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& record) const override;
    virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const override;

public:
	point3 m_center;
	double m_radius;
    shared_ptr<material> m_material_ptr;

private:
    static void getUV(const point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};

bool sphere::hit(const ray& r, double tMin, double tMax, hitRecord& record) const {
	vec3 oc = r.getOrigin() - m_center;
	double a = r.getDirection().lengthSquared();
    double half_b = dot(oc, r.getDirection());
    double c = oc.lengthSquared() - m_radius * m_radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0) return false; // No intersection
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < tMin || tMax < root) {
        root = (-half_b + sqrtd) / a;
        if (root < tMin || tMax < root)
            return false;
    }

    record.t = root;
    record.point = r.resize(record.t);
    vec3 outwardNormal = (record.point - m_center) / m_radius;
    record.setFaceNormal(r, outwardNormal);
    getUV(outwardNormal, record.u, record.v);
    record.material_ptr = m_material_ptr;

    return true;
}

bool sphere::getAABB(double startTime, double endTime, aabb& outputBox) const {
    outputBox = aabb(
        m_center - vec3(m_radius, m_radius, m_radius),
        m_center + vec3(m_radius, m_radius, m_radius)
    );

    return true;
}
