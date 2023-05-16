#pragma once

#include "hittable.h"

class sphere : public hittable {
public:
    sphere() : m_center(vec3(0.0, 0.0, 0.0)), m_radius(0.0) {}
	sphere(point3 center, double r, shared_ptr<material> material_ptr)
        : m_center(center), m_radius(r), m_material_ptr(material_ptr) {};

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& record) const override;

public:
	point3 m_center;
	double m_radius;
    shared_ptr<material> m_material_ptr;
};

bool sphere::hit(const ray& ray, double tMin, double tMax, hitRecord& record) const {
	vec3 oc = ray.getOrigin() - m_center;
	double a = ray.getDirection().lengthSquared();
    double half_b = dot(oc, ray.getDirection());
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
    record.point = ray.resize(record.t);
    vec3 outwardNormal = (record.point - m_center) / m_radius;
    record.setFaceNormal(ray, outwardNormal);
    record.material_ptr = m_material_ptr;

    return true;
}