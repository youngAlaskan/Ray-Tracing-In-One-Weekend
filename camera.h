#pragma once

#include "utils.h"

class camera {
public:
	camera(
		point3 lookFrom,
		point3 lookAt,
		vec3 vUp,
		double vFovDegrees,
		double aspectRatio
	) {
		double theta = degreesToRadians(vFovDegrees);
		double h = tan(theta / 2.0);
		double viewportHeight = 2.0 * h;
		double viewportWidth = aspectRatio * viewportHeight;

		vec3 w = unitVector(lookFrom - lookAt);
		vec3 u = unitVector(cross(vUp, w));
		vec3 v = cross(w, u);

		origin = lookFrom;
		horizontal = viewportWidth * u;
		vertical = viewportHeight * v;
		lowerLeftCorner = origin - horizontal / 2.0 - vertical / 2.0 - w;
	}

	ray getRay(double s, double t) const {
		return ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin);
	}

private:
	point3 origin;
	point3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
};