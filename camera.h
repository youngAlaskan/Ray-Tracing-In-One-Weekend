#pragma once

#include "utils.h"

class camera {
public:
	camera(
		point3 lookFrom,
		point3 lookAt,
		vec3 vUp,
		double vFovDegrees,
		double aspectRatio,
		double aperture,
		double focusDistance,
		double startTime = 0.0,
		double endTime = 0.0
	) {
		double theta = degreesToRadians(vFovDegrees);
		double h = tan(theta / 2.0);
		double viewportHeight = 2.0 * h;
		double viewportWidth = aspectRatio * viewportHeight;

		m_w = unitVector(lookFrom - lookAt);
		m_u = unitVector(cross(vUp, m_w));
		m_v = cross(m_w, m_u);

		m_origin = lookFrom;
		m_horizontal = focusDistance * viewportWidth * m_u;
		m_vertical = focusDistance * viewportHeight * m_v;
		m_lowerLeftCorner = m_origin - m_horizontal / 2.0 - m_vertical / 2.0 - focusDistance * m_w;
	
		m_lensRadius = aperture / 2.0;
		m_startTime = startTime;
		m_endTime = endTime;
	}

	ray getRay(double s, double t) const {
		vec3 rd = m_lensRadius * randomInUnitDisk();
		vec3 m_offset = m_u * rd.x() + m_v * rd.y();

		return ray(
			m_origin + m_offset,
			m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - m_offset,
			randomDouble(m_startTime, m_endTime)
		);
	}

private:
	point3 m_origin;
	point3 m_lowerLeftCorner;
	vec3 m_horizontal;
	vec3 m_vertical;
	vec3 m_u, m_v, m_w;
	double m_lensRadius;
	double m_startTime, m_endTime; // Shutter open, close times
};