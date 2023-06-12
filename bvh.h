#pragma once

#include "utils.h"

#include "hittable.h"
#include "hittableList.h"

#include <algorithm>

class bvhNode : public hittable {
public:
	bvhNode() {}

	bvhNode(const hittableList& entities, double startTime, double endTime)
		: bvhNode(std::vector<shared_ptr<hittable>>(entities.m_objects), size_t(0), entities.m_objects.size(), startTime, endTime) {}

	bvhNode(
		std::vector<shared_ptr<hittable>>& srcObjects, size_t start, size_t end, double startTime, double endTime
	);

	virtual bool hit(
		const ray& r, double tMin, double tMax, hitRecord& record
	) const override;

	virtual bool getAABB(
		double startTime, double endTime, aabb& outputBox
	) const override;

public:
	shared_ptr<bvhNode> m_left;
	shared_ptr<bvhNode> m_right;
	shared_ptr<hittable> m_object; // If node is a leaf it contains a single object
	aabb m_box;
};

inline bool boxCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
	aabb boxA;
	aabb boxB;

	if (!a->getAABB(0.0, 0.0, boxA) || !b->getAABB(0.0, 0.0, boxB))
		std::cerr << "No bounding box in bvhNode constructor.\n";

	return boxA.getMin()[axis] < boxB.getMin()[axis];
}

bool compareBoxX(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return boxCompare(a, b, 0);
}

bool compareBoxY(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return boxCompare(a, b, 1);
}

bool compareBoxZ(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return boxCompare(a, b, 2);
}

bvhNode::bvhNode(std::vector<shared_ptr<hittable>>& srcObjects,
	size_t start, size_t end, double startTime, double endTime) {

	std::vector<shared_ptr<hittable>> objects = srcObjects; // modifiable objects vector

	// Choose random axis to split evenly down
	int axis = randomInt(0, 2);
	auto comparator = (axis == 0) ? compareBoxX : ((axis == 1) ? compareBoxY : compareBoxZ);

	size_t objectSpan = end - start;

	switch (objectSpan) {
	case 0:
		return;
	case 1:
		m_object = objects[start];
		m_object->getAABB(startTime, endTime, m_box);
		return;
	case 2:
		if (comparator(objects[start], objects[start + 1])) {
			m_left = make_shared<bvhNode>(objects[start], startTime, endTime);
			m_right = make_shared<bvhNode>(objects[start + 1], startTime, endTime);
		}
		else {
			m_left = make_shared<bvhNode>(objects[start + 1], startTime, endTime);
			m_right = make_shared<bvhNode>(objects[start], startTime, endTime);
		}
		break;
	default:
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		int mid = start + objectSpan / 2;
		m_left = make_shared<bvhNode>(objects, start, mid, startTime, endTime);
		m_right = make_shared<bvhNode>(objects, mid, end, startTime, endTime);
		break;
	}

	aabb boxLeft, boxRight;

	if (!m_left->getAABB(startTime, endTime, boxLeft)
		|| !m_right->getAABB(startTime, endTime, boxRight)
	) std::cerr << "No bounding box in bvhNode constructor.\n";

	m_box = aabb::surroundingBox(m_left->m_box, m_right->m_box);
}

bool bvhNode::hit(const ray& r, double tMin, double tMax, hitRecord& record) const {
	if (!m_box.hit(r, tMin, tMax)) return false;

	if (m_object) return m_object->hit(r, tMin, tMax, record);

	bool hitLeft = m_left->hit(r, tMin, tMax, record);
	bool hitRight = m_right->hit(r, tMin, hitLeft ? record.t : tMax, record);

	return hitLeft || hitRight;
}

bool bvhNode::getAABB(double startTime, double endTime, aabb& outputBox) const {
	outputBox = m_box;
	return true;
}
