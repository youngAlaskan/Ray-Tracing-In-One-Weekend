#pragma once

#include "hittable.h"
#include "aabb.h"

#include <memory>
#include <vector>

class hittableList : public hittable {
public:
	hittableList() {}
	hittableList(std::shared_ptr<hittable> object) { add(object); }

	void clear() { m_objects.clear(); }
	void add(std::shared_ptr<hittable> object) { m_objects.push_back(object); }

	virtual bool hit(
		const ray& ray, double tMin, double tMax, hitRecord& record
	) const override;

	virtual bool getAABB(
		double startTime, double endTime, aabb& outputBox
	) const override;

public:
	std::vector<std::shared_ptr<hittable>> m_objects;
};

bool hittableList::hit(const ray& ray, double tMin, double tMax, hitRecord& record) const {
	hitRecord tempRecord;
	bool hasHit = false;
	double currentClosest = tMax;

	for (const std::shared_ptr<hittable>& object : m_objects) {
		if (object->hit(ray, tMin, currentClosest, tempRecord)) {
			hasHit = true;
			currentClosest = tempRecord.t;
			record = tempRecord;
		}
	}

	return hasHit;
}

bool hittableList::getAABB(double startTime, double endTime, aabb& outputBox) const {
	if (m_objects.empty()) return false;

	aabb tempBox;
	bool firstBox = true;

	for (const auto& object : m_objects) {
		if (!object->getAABB(startTime, endTime, tempBox)) return false;
		outputBox = firstBox ? tempBox : aabb::surroundingBox(outputBox, tempBox);
		firstBox = false;
	}

	return true;
}
