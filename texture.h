#pragma once

#include "utils.h"
#include "rtw_stb_image.h"
#include "perlin.h"

#include <iostream>

class texture {
public:
	virtual color getValue(double u, double v, const point3& p) const = 0;
};

class imageTexture : public texture {
public:
	imageTexture(const char* filename) {
		int componentsPerPixel = s_bytesPerPixel;

		m_data = stbi_load(
			filename, &m_width, &m_height, &componentsPerPixel, componentsPerPixel
		);

		if (!m_data) {
			std::cerr << "ERROR: Could not load texture image file '" << filename << "',\n";
			m_width = m_height = 0;
		}

		m_bytesPerScanline = s_bytesPerPixel * m_width;
	}

	~imageTexture() {
		delete[] m_data;
	}

	virtual color getValue(double u, double v, const point3& p) const override {
		// If we have no texture data, then return solid magenta
		if (!m_data) return color(0.0, 1.0, 1.0);

		// Clamp input tex coords to [0,1] x [1,0]
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to img coords

		int i = static_cast<int>(u * m_width);
		int j = static_cast<int>(v * m_height);

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i >= m_width)  i = m_width - 1;
		if (j >= m_height) j = m_height - 1;

		const double colorScale = 1.0 / 255.0;
		unsigned char* pixel = m_data + j * m_bytesPerScanline + i * s_bytesPerPixel;

		return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}

public:
	const static int s_bytesPerPixel = 3;

private:
	unsigned char* m_data;
	int m_width, m_height;
	int m_bytesPerScanline;
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
		// Clamp u and v to 0.0 - 1.0
		u -= floor(u);
		v -= floor(v);

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
