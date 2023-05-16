#pragma once

#include "utils.h"

#include "material.h"

struct hitRecord {
	point3 point;
	vec3 normal;
	shared_ptr<material> material_ptr;
	double t;
	bool isFrontFace;

	inline void setFaceNormal(const ray& ray, const vec3& outwardNormal) {
		isFrontFace = dot(ray.getDirection(), outwardNormal);
		normal = isFrontFace ? outwardNormal : -outwardNormal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& ray, double tMin, double tMax, hitRecord& record) const = 0;
};