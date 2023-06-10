#pragma once

#include "utils.h"
#include "hittable.h"
#include "hittableList.h"

struct vertex {
	point3 m_location;
	vec3 m_normal;
	vec2 m_texCoord;

	vertex() {}

	vertex(point3 location, vec3 normal, vec2 texCoord)
		: m_location(location), m_normal(normal), m_texCoord(texCoord) {}
};

struct triangle {
	vertex* m_vertices;

	triangle() : m_vertices(nullptr) {}

	triangle(vertex v0, vertex v1, vertex v2) {
		m_vertices = new vertex[3];
		m_vertices[0] = v0;
		m_vertices[1] = v1;
		m_vertices[2] = v2;
	}
	
	triangle(point3* vertices, vec3* normals, vec2* texCoords) {
		m_vertices = new vertex[3];
		for (int i = 0; i < 3; i++) {
			m_vertices[i] = vertex(vertices[i], normals[i], texCoords[i]);
		}
	}
};

class TriangleMesh : public hittable {
public:
	TriangleMesh() : m_trianglesN(0), m_verticesN(0), m_triangles(nullptr) {}

	TriangleMesh(int verticesN, int trianglesN, triangle* triangles)
		: m_verticesN(verticesN), m_trianglesN(trianglesN), m_triangles(triangles) {}

	virtual bool hit(const ray& ray, double tMin, double tMax, hitRecord& record) const = 0;
	virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const = 0;

	~TriangleMesh() {
		if (m_triangles) delete[] m_triangles;
	}

public:
	int m_verticesN, m_trianglesN;
	triangle* m_triangles;
	shared_ptr<material> m_material_ptr;
	aabb m_aabb;
};

class aaRectangle : public TriangleMesh {
public:
	aaRectangle() : m_x0(0.0), m_x1(0.0), m_y0(0.0), m_y1(0.0), m_z0(0.0), m_z1(0.0) {}

	aaRectangle(point3 v0, point3 v1, shared_ptr<material> mat)
		: m_x0(v0.x()), m_x1(v1.x()), m_y0(v0.y()), m_y1(v1.y()), m_z0(v0.z()), m_z1(v1.z()) {
		m_verticesN = 4;
		m_trianglesN = 2;
		
		m_triangles = new triangle[2];

		m_material_ptr = mat;

		vec3 normal;
		vertex lowL, upL, upR, lowR;

		if (m_z0 == m_z1) {
			normal = unitVector(cross(vec3(m_x1 - m_x0, 0.0, 0.0), vec3(0.0, m_y1 - m_y0, 0.0)));

			lowL = vertex(v0, normal, vec2(0.0, 0.0));
			upL  = vertex(point3(m_x0, m_y1, m_z0), normal, vec2(0.0, 1.0));
			upR  = vertex(v1, normal, vec2(1.0, 1.0));
			lowR = vertex(point3(m_x1, m_y0, m_z0), normal, vec2(1.0, 0.0));
		} else if (m_y0 == m_y1) {
			normal = unitVector(cross(vec3(m_x1 - m_x0, 0.0, 0.0), vec3(0.0, 0.0, m_z1 - m_z0)));

			lowL = vertex(v0, normal, vec2(0.0, 0.0));
			upL  = vertex(point3(m_x0, m_y0, m_z1), normal, vec2(0.0, 1.0));
			upR  = vertex(v1, normal, vec2(1.0, 1.0));
			lowR = vertex(point3(m_x1, m_y0, m_z0), normal, vec2(1.0, 0.0));
		} else {
			normal = unitVector(cross(vec3(0.0, 0.0, m_z1 - m_z0), vec3(0.0, m_y1 - m_y0, 0.0)));

			lowL = vertex(v0, normal, vec2(0.0, 0.0));
			upL  = vertex(point3(m_x0, m_y1, m_z0), normal, vec2(0.0, 1.0));
			upR  = vertex(v1, normal, vec2(1.0, 1.0));
			lowR = vertex(point3(m_x0, m_y0, m_z1), normal, vec2(1.0, 0.0));
		}

		m_triangles[0] = triangle(lowL, upL, upR);
		m_triangles[1] = triangle(lowL, upR, lowR);
	}
	
	virtual bool hit(const ray& ray, double tMin, double tMax, hitRecord& record) const override;

	virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const override {
		// The bounding box must have non-zero width in each dimension, so pad the Z
		// dimension a small amount.
		if (m_z0 == m_z1) {
			outputBox = aabb(point3(m_x0, m_y0, m_z0 - 0.0001), point3(m_x1, m_y1, m_z1 + 0.0001));
		} else if (m_y0 == m_y1) {
			outputBox = aabb(point3(m_x0, m_y0 - 0.0001, m_z0), point3(m_x1, m_y1 + 0.0001, m_z1));
		} else {
			outputBox = aabb(point3(m_x0 - 0.0001, m_y0, m_z0), point3(m_x1 + 0.0001, m_y1, m_z1));
		}

		return true;
	}

public:
	double m_x0, m_x1, m_y0, m_y1, m_z0, m_z1;
};

bool aaRectangle::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const {
	double t, x, y, z;

	if (m_z0 == m_z1) {
		t = (m_z0 - r.getOrigin().z()) / r.getDirection().z();
		if (t < t_min || t > t_max) return false;

		x = r.getOrigin().x() + t * r.getDirection().x();
		y = r.getOrigin().y() + t * r.getDirection().y();

		if (x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1) return false;

		rec.u = (x - m_x0) / (m_x1 - m_x0);
		rec.v = (y - m_y0) / (m_y1 - m_y0);
	} else if (m_y0 == m_y1) {
		t = (m_y0 - r.getOrigin().y()) / r.getDirection().y();
		if (t < t_min || t > t_max) return false;

		x = r.getOrigin().x() + t * r.getDirection().x();
		z = r.getOrigin().z() + t * r.getDirection().z();

		if (x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1) return false;

		rec.u = (x - m_x0) / (m_x1 - m_x0);
		rec.v = (z - m_z0) / (m_z1 - m_z0);
	} else {
		t = (m_x0 - r.getOrigin().x()) / r.getDirection().x();
		if (t < t_min || t > t_max) return false;

		y = r.getOrigin().y() + t * r.getDirection().y();
		z = r.getOrigin().z() + t * r.getDirection().z();

		if (y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1) return false;

		rec.u = (y - m_y0) / (m_y1 - m_y0);
		rec.v = (z - m_z0) / (m_z1 - m_z0);
	}

	rec.t = t;
	rec.setFaceNormal(r, m_triangles->m_vertices->m_normal);
	rec.material_ptr = m_material_ptr;
	rec.point = r.resize(t);
	return true;
}

class aaBox : public TriangleMesh {
public:
	aaBox() {}
	aaBox(const point3& p0, const point3& p1, shared_ptr<material> mat_ptr);

	virtual bool hit(const ray& ray, double tMin, double tMax, hitRecord& record) const override;

	virtual bool getAABB(double startTime, double endTime, aabb& outputBox) const override {
		outputBox = aabb(m_boxMin, m_boxMax);
		return true;
	}

public:
	point3 m_boxMin;
	point3 m_boxMax;
	hittableList m_sides;
};

aaBox::aaBox(const point3& p0, const point3& p1, shared_ptr<material> mat_ptr) {
	m_boxMin = p0;
	m_boxMax = p1;

	m_sides.add(make_shared<aaRectangle>(p0 + vec3(0.0, 0.0, p1.z() - p0.z()), p1, mat_ptr));
	m_sides.add(make_shared<aaRectangle>(p0, p1 - vec3(0.0, 0.0, p1.z() - p0.z()), mat_ptr));
	
	m_sides.add(make_shared<aaRectangle>(p0 + vec3(0.0, p1.y() - p0.y(), 0.0), p1, mat_ptr));
	m_sides.add(make_shared<aaRectangle>(p0, p1 - vec3(0.0, p1.y() - p0.y(), 0.0), mat_ptr));
	
	m_sides.add(make_shared<aaRectangle>(p0 + vec3(p1.x() - p0.x(), 0.0, 0.0), p1, mat_ptr));
	m_sides.add(make_shared<aaRectangle>(p0, p1 - vec3(p1.x() - p0.x(), 0.0, 0.0), mat_ptr));
}

bool aaBox::hit(const ray& r, double tMin, double tMax, hitRecord& record) const {
	return m_sides.hit(r, tMin, tMax, record);
}
