#pragma once

#include "utils.h"

class aabb {
public:
	aabb() {}
	aabb(const point3& a, const point3& b) : m_min(a), m_max(b) {}

	point3 getMin() const { return m_min; }
	point3 getMax() const { return m_max; }

	static aabb surroundingBox(aabb startBox, aabb endBox) {
		point3 small(fmin(startBox.getMin().x(), endBox.getMin().x()),
			fmin(startBox.getMin().y(), endBox.getMin().y()),
			fmin(startBox.getMin().z(), endBox.getMin().z()));

		point3 big(fmax(startBox.getMax().x(), endBox.getMax().x()),
			fmax(startBox.getMax().y(), endBox.getMax().y()),
			fmax(startBox.getMax().z(), endBox.getMax().z()));

		return aabb(small, big);
	}

	bool hit(const ray& r, double tMin, double tMax) const {
		for (int i = 0; i < 3; i++) {
			double element = r.getOrigin()[i];
			double direction = r.getDirection()[i];
			double tClose = fmin((m_min[i] - element) / direction,
				(m_max[i] - element) / direction);
			double tFar = fmax((m_min[i] - element) / direction,
				(m_max[i] - element) / direction);
			tMin = fmax(tClose, tMin);
			tMax = fmin(tFar, tMax);
			if (tMax <= tMin) return false;
		}

		return true;
	}

public:
	point3 m_min;
	point3 m_max;
};

inline bool aabb::hit(const ray& r, double tMin, double tMax) const {
	for (int i = 0; i < 3; i++) {
		double inverseD = 1.0 / r.getDirection()[i];
		double tClose = (getMin()[i] - r.getOrigin()[i]) * inverseD;
		double tFar = (getMax()[i] - r.getOrigin()[i]) * inverseD;
		if (inverseD < 0.0) std::swap(tClose, tFar);
		tMin = tClose > tMin ? tClose : tMin;
		tMax = tFar < tMax ? tFar : tMax;
		if (tMax <= tMin) return false;
	}

	return true;
}
