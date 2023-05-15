#pragma once

#include "ray.h"

struct hitRecord {
	point3 point;
	vec3 normal;
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