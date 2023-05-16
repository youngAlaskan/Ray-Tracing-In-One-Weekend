#pragma once

#include "utils.h"

class material {
public:
	virtual bool scatter(
		const ray& inRay, const point3& point, const vec3& normal, color& attenuation, ray& scattered
	) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color& albedo) : m_albedo(albedo) {}

	virtual bool scatter(
		const ray& inRay, const point3& point, const vec3& normal, color& attenuation, ray& scattered
	) const override {
		vec3 scatterDir = normal + randomUnitVector();

		if (scatterDir.nearZero()) scatterDir = normal;

		scattered = ray(point, scatterDir);
		attenuation = m_albedo;
		return true;
	}

public:
	color m_albedo;
};

class metal : public material {
public:
	metal(const color& albedo, double fuzz) : m_albedo(albedo), m_reflectionFuzz(fuzz < 1.0 ? fuzz : 1.0) {}

	virtual bool scatter(
		const ray& inRay, const point3& point, const vec3& normal, color& attenuation, ray& scattered
	) const override {
		vec3 reflected = reflect(unitVector(inRay.getDirection()), normal);
		scattered = ray(point, reflected + m_reflectionFuzz * randomInUnitSphere());
		attenuation = m_albedo;
		return dot(scattered.getDirection(), normal) > 0.0;
	}

public:
	color m_albedo;
	double m_reflectionFuzz;
};
