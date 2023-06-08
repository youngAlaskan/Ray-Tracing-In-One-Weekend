#pragma once

#include "utils.h"
#include "perlin.h"

class texture {
public:
	virtual color getValue(double u, double v, const point3& p) const = 0;
};

class solidColor : public texture {
public:
	solidColor() {}
	solidColor(color c) : colorValue(c) {}

	solidColor(double red, double green, double blue)
		: solidColor(color(red, green, blue)) {}

	virtual color getValue(double u, double v, const point3& p) const override {
		return colorValue;
	}

private:
	color colorValue;
};

class checkerTexture : public texture {
public:
	checkerTexture() {}

	checkerTexture(shared_ptr<texture> even, shared_ptr<texture> odd)
		: m_even(even), m_odd(odd) {}

	checkerTexture(color even, color odd)
		: m_even(make_shared<solidColor>(even)), m_odd(make_shared<solidColor>(odd)) {}

	virtual color getValue(double u, double v, const point3& p) const override {
		double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		return sines < 0 ? m_odd->getValue(u, v, p) : m_even->getValue(u, v, p);
	}

public:
	shared_ptr<texture> m_odd;
	shared_ptr<texture> m_even;
};

class perlinTexture : public texture {
public:
	perlinTexture() : m_scale(1.0) {}
	perlinTexture(double scale) : m_scale(scale) {}

	virtual color getValue(double u, double v, const point3& p) const override {
		return color(1.0, 1.0, 1.0) * 0.5 * (1.0 + sin(m_scale * p.z() + 10.0 * noise.turbulant(p)));
	}

public:
	perlin noise;
	double m_scale;
};
