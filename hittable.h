#pragma once

#include "utils.h"

#include "material.h"
#include "aabb.h"

struct hitRecord {
	point3 point;
	vec3 normal;
	shared_ptr<material> material_ptr;
	double t = 0.0;
	bool isFrontFace = true;

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