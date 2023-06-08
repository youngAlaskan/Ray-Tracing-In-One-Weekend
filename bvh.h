#pragma once

#include "utils.h"

#include "hittable.h"
#include "hittableList.h"

#include <algorithm>

class bvhNode : hittable {
public:
	bvhNode() {}

	bvhNode(shared_ptr<hittable> object, double startTime, double endTime) {
		m_object = object;
		m_object->getAABB(startTime, endTime, m_box);
	}

	bvhNode(
		std::vector<shared_ptr<hittable>>& srcObjects, size_t start, size_t end, double startTime, double endTime
	) {}

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

bvhNode::bvhNode(
	std::vector<shared_ptr<hittable>>& srcObjects, size_t start, size_t end, double startTime, double endTime
) {
	std::vector<shared_ptr<hittable>> objects = srcObjects;

	int axis = randomInt(0, 2);
	auto comparator = (axis == 0) ? compareBoxX
		: ((axis == 1) ? compareBoxY : compareBoxZ);

	size_t objectSpan = end - start;

	switch (objectSpan) {
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
	}

	aabb boxLeft, boxRight;

	if (!m_left->getAABB(startTime, endTime, boxLeft)
		|| !m_right->getAABB(startTime, endTime, boxRight)
	) std::cerr << "No bounding box in bvhNode constructor.\n";

	m_box = aabb::surroundingBox(boxLeft, boxRight);
}

bool bvhNode::hit(const ray& r, double tMin, double tMax, hitRecord& record) const {
	if (!m_box.hit(r, tMin, tMax)) return false;

	bool hitLeft = m_left->hit(r, tMin, tMax, record);
	bool hitRight = m_right->hit(r, tMin, hitLeft ? record.t : tMax, record);

	return hitLeft || hitRight;
}

bool bvhNode::getAABB(double startTime, double endTime, aabb& outputBox) const {
	outputBox = m_box;
	return true;
}

class bvh {
public:
	bvh() {}

	bvh(bvhNode& node) : m_root(node) {}

	bvh(std::vector<shared_ptr<hittable>>& srcObjects, size_t start, size_t end, double startTime, double endTime)
		: m_root(srcObjects, start, end, startTime, endTime) {}

	bvh(const hittableList list, double startTime, double endTime)
		: m_root(list.getObjects(), size_t(0), list.m_objects.size(), startTime, endTime) {}

public:
	bvhNode m_root;
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
