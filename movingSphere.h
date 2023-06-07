#pragma once

#include "utils.h"
#include "sphere.h"
#include "aabb.h"

class movingSphere : public sphere {
public:
	movingSphere() : m_startCenter(), m_endCenter(), m_startTime(0.0), m_endTime(0.0), m_radius(0.0), m_mat_ptr(nullptr) {}
	movingSphere(point3 startCenter, point3 endCenter, double startTime, double endTime, double radius, shared_ptr<material> mat)
		: m_startCenter(startCenter), m_endCenter(endCenter), m_startTime(startTime), m_endTime(endTime), m_radius(radius), m_mat_ptr(mat) {};
	
	virtual bool hit(
		const ray& r, double tMin, double tMax, hitRecord& record
    ) const override;
	
    virtual bool getAABB(
        double startTime, double endTime, aabb& outputBox
    ) const override;
    
    point3 getCenter(double time) const;
public:
	point3 m_startCenter, m_endCenter;
	double m_startTime, m_endTime;
	double m_radius;
	shared_ptr<material> m_mat_ptr;
};

point3 movingSphere::getCenter(double time) const {
	return m_startCenter + ((time - m_startTime) / (m_endTime - m_startTime)) * (m_endCenter - m_startCenter);
}

bool movingSphere::hit(const ray& r, double tMin, double tMax, hitRecord& record) const {
    vec3 oc = r.getOrigin() - getCenter(r.getTime());
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
    vec3 outwardNormal = (record.point - getCenter(r.getTime())) / m_radius;
    record.setFaceNormal(r, outwardNormal);
    record.material_ptr = m_mat_ptr;

    return true;
}

bool movingSphere::getAABB(double startTime, double endTime, aabb& outputBox) const {
    aabb startBox(
        getCenter(startTime) - vec3(m_radius, m_radius, m_radius),
        getCenter(startTime) + vec3(m_radius, m_radius, m_radius)
    );

    aabb endBox(
        getCenter(endTime) - vec3(m_radius, m_radius, m_radius),
        getCenter(endTime) + vec3(m_radius, m_radius, m_radius)
    );

    outputBox = aabb::surroundingBox(startBox, endBox);

    return true;
}
