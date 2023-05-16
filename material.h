#pragma once

#include "utils.h"

class material {
public:
	virtual bool scatter(
		const ray& inRay, const point3& point, const vec3& normal, const bool& isFrontFace, color& attenuation, ray& scattered
	) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color& albedo) : m_albedo(albedo) {}

	virtual bool scatter(
		const ray& inRay, const point3& point, const vec3& normal, const bool& isFrontFace, color& attenuation, ray& scattered
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
		const ray& inRay, const point3& point, const vec3& normal, const bool& isFrontFace, color& attenuation, ray& scattered
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

class dielectric : public material {
public:
	dielectric(double refractionIndex) : m_refractionIndex(refractionIndex) {}

	virtual bool scatter(
		const ray& inRay, const point3& point, const vec3& normal, const bool& isFrontFace, color& attenuation, ray& scattered
	) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double refractionRatio = isFrontFace ? (1.0 / m_refractionIndex) : m_refractionIndex;

		vec3 unitDir = unitVector(inRay.getDirection());
		double cosTheta = fmin(dot(-unitDir, normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		vec3 direction = cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble() ?
			reflect(unitDir, normal) : refract(unitDir, normal, cosTheta, refractionRatio);

		scattered = ray(point, direction);
		
		return true;
	}
public:
	double m_refractionIndex;
private:
	static double reflectance(double cosine, double refractionRatio) {
		// Use Schlick's approximation for reflectance.
		double r0 = (1.0 - refractionRatio) / (1.0 + refractionRatio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow(1.0 - cosine, 5);
	}
};
