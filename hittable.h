#pragma once

#include "utils.h"

#include "material.h"
#include "aabb.h"

struct hitRecord {
	// Object Information
	point3 point;
	vec3 normal;
	shared_ptr<material> material_ptr;
	double t = 0.0;
	bool isFrontFace = true;
	// Texture Coords
	double u = 0.0;
	double v = 0.0;

	inline void setFaceNormal(const ray& ray, const vec3& outwardNormal) {
		isFrontFace = dot(ray.getDirection(), outwardNormal) < 0.0;
		normal = isFrontFace ? outwardNormal : -outwardNormal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& ray, double tMin, double tMax, hitRecord& record) const = 0;
	virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const = 0;
};

class translation : public hittable {
public:
	translation(shared_ptr<hittable> p, const vec3& displacement)
		: m_ptr(p), m_offset(displacement) {}

	virtual bool hit(
		const ray& r, double tMin, double tMax, hitRecord& record) const override;

	virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const override;

public:
	shared_ptr<hittable> m_ptr;
	vec3 m_offset;
};

bool translation::hit(const ray& r, double tMin, double tMax, hitRecord& record) const {
	ray movedR(r.getOrigin() - m_offset, r.getDirection(), r.getTime());
	if (!m_ptr->hit(movedR, tMin, tMax, record))
		return false;

	record.point += m_offset;
	record.setFaceNormal(movedR, record.normal);

	return true;
}

bool translation::getAABB(double startTime, double endTime, aabb& outputBox) const {
	if (!m_ptr->getAABB(startTime, endTime, outputBox))
		return false;

	outputBox = aabb(
		outputBox.getMin() + m_offset,
		outputBox.getMax() + m_offset);

	return true;
}

class pan : public hittable {
public:
	pan(shared_ptr<hittable> ptr, double angle);

	virtual bool hit(
		const ray& r, double tMin, double tMax, hitRecord& record) const override;

	virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const override {
		outputBox = m_bBox;
		return m_hasBox;
	}

public:
	shared_ptr<hittable> m_ptr;
	double m_sinTheta;
	double m_cosTheta;
	bool m_hasBox;
	aabb m_bBox;
};

pan::pan(shared_ptr<hittable> ptr, double angle) : m_ptr(ptr) {
	auto radians = degreesToRadians(angle);
	m_sinTheta = sin(radians);
	m_cosTheta = cos(radians);
	m_hasBox = ptr->getAABB(0.0, 1.0, m_bBox);

	point3 min(infinity, infinity, infinity);
	point3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = i * m_bBox.getMax().x() + (1 - i) * m_bBox.getMin().x();
				auto y = j * m_bBox.getMax().y() + (1 - j) * m_bBox.getMin().y();
				auto z = k * m_bBox.getMax().z() + (1 - k) * m_bBox.getMin().z();

				auto newX = m_cosTheta * x + m_sinTheta * z;
				auto newZ = -m_sinTheta * x + m_cosTheta * z;

				vec3 tester(newX, y, newZ);

				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	m_bBox = aabb(min, max);
}

bool pan::hit(const ray& r, double tMin, double tMax, hitRecord& record) const {
	auto origin = r.getOrigin();
	auto direction = r.getDirection();

	origin[0] = m_cosTheta * r.getOrigin()[0] - m_sinTheta * r.getOrigin()[2];
	origin[2] = m_sinTheta * r.getOrigin()[0] + m_cosTheta * r.getOrigin()[2];

	direction[0] = m_cosTheta * r.getDirection()[0] - m_sinTheta * r.getDirection()[2];
	direction[2] = m_sinTheta * r.getDirection()[0] + m_cosTheta * r.getDirection()[2];

	ray rotatedR(origin, direction, r.getTime());

	if (!m_ptr->hit(rotatedR, tMin, tMax, record))
		return false;

	auto p = record.point;
	auto normal = record.normal;

	p[0] = m_cosTheta * record.point[0] + m_sinTheta * record.point[2];
	p[2] = -m_sinTheta * record.point[0] + m_cosTheta * record.point[2];

	normal[0] = m_cosTheta * record.normal[0] + m_sinTheta * record.normal[2];
	normal[2] = -m_sinTheta * record.normal[0] + m_cosTheta * record.normal[2];

	record.point = p;
	record.setFaceNormal(rotatedR, normal);

	return true;
}

// class roll : public hittable {
// 
// };
// 
// class tilt : public hittable {
// 
// };
