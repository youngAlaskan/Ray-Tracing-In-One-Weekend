#pragma once

#include"hittable.h"

#include<memory>
#include<vector>

class hittableList : public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;
public:
	hittableList() {}
	hittableList(std::shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& ray, double tMin, double tMax, hitRecord& record) const override;
};

bool hittableList::hit(const ray& ray, double tMin, double tMax, hitRecord& record) const {
	hitRecord tempRecord;
	bool hasHit = false;
	double currentClosest = tMax;

	for (const std::shared_ptr<hittable>& object : objects) {
		if (object->hit(ray, tMin, currentClosest, tempRecord)) {
			hasHit = true;
			currentClosest = tempRecord.t;
			record = tempRecord;
		}
	}

	return hasHit;
}
