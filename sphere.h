#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 center, double r) : m_center(center), m_radius(r) {};

	virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& record) const override;

private:
	point3 m_center;
	double m_radius;
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
    record.normal = (record.point - m_center) / m_radius;

    return true;
}