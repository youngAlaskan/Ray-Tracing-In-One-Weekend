#pragma once

#include "utils.h"

class camera {
public:
	camera() {
		double aspectRatio = 16.0 / 9.0;
		double viewportHeight = 2.0;
		double viewportWidth = aspectRatio * viewportHeight;
		double focalLength = 1.0;

		origin = point3(0.0, 0.0, 0.0);
		horizontal = vec3(viewportWidth, 0.0, 0.0);
		vertical = vec3(0.0, viewportHeight, 0.0);
		lowerLeftCorner = origin - horizontal * 0.5 - vertical * 0.5 - vec3(0.0, 0.0, focalLength);
	}

	ray getRay(double u, double v) const {
		return ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}

private:
	point3 origin;
	point3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
};