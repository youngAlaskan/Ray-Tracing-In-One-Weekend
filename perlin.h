#pragma once

#include "utils.h"

class perlin {
public:
	perlin() {
		m_randVec = new vec3[m_pointCount];
		for (int i = 0; i < m_pointCount; ++i) {
			m_randVec[i] = unitVector(vec3::random(-1.0, 1.0));
		}

		m_permX = perlinGeneratePerm();
		m_permY = perlinGeneratePerm();
		m_permZ = perlinGeneratePerm();
	}

	~perlin() {
		delete[] m_randVec;
		delete[] m_permX;
		delete[] m_permY;
		delete[] m_permZ;
	}

	double genNoise(const point3& p) const {
		double u = p.x() - floor(p.x());
		double v = p.y() - floor(p.y());
		double w = p.z() - floor(p.z());

		int i = static_cast<int>(floor(p.x()));
		int j = static_cast<int>(floor(p.y()));
		int k = static_cast<int>(floor(p.z()));
		vec3 c[2][2][2]; // Explained in trilinearInterpolation

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = m_randVec[
						m_permX[(i + di) & 255] ^
						m_permY[(j + dj) & 255] ^
						m_permZ[(k + dk) & 255]
					];

		return trilinearInterpolation(c, u, v, w);
	}

	double turbulant(const point3& p, int depth = 7) const {
		double accum = 0.0;
		point3 pTemp = p;
		double weight = 1.0;

		for (int i = 0; i < depth; i++) {
			accum += weight * genNoise(pTemp);
			weight *= 0.5;
			pTemp *= 2.0;
		}

		return fabs(accum);
	}

private:
	static int* perlinGeneratePerm() {
		int* p = new int[m_pointCount];

		for (int i = 0; i < perlin::m_pointCount; i++) {
			p[i] = i;
		}

		permute(p, m_pointCount);

		return p;
	}

	static void permute(int* p, int n) {
		for (int i = n - 1; i > 0; i--) {
			int target = randomInt(0, i);
			int temp = p[i];
			p[i] = p[target];
			p[target] = temp;
		}
	}

	static double trilinearInterpolation(vec3 c[2][2][2], double u, double v, double w) {
		// c: 2 x 2 x 2 cube
		//		/---/---/ |
		//     /- -/- -/| |
		//    /---/---/ |/|
		//    |   |   | | |
		//    |---|---|/|/
		//    |   |   | /
		//    |---|---|/

		double uu = u * u * (3.0 - 2.0 * u);
		double vv = v * v * (3.0 - 2.0 * v);
		double ww = w * w * (3.0 - 2.0 * w);
		double accum = 0.0;

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					vec3 weight(u - i, v - j, w - k);
					accum += (i * uu + (1 - i) * (1 - uu))
						   * (j * vv + (1 - j) * (1 - vv))
						   * (k * ww + (1 - k) * (1 - ww))
						   * dot(c[i][j][k], weight);
				}
			}
		}

		return accum;
	}

private:
	static const int m_pointCount = 256;
	vec3* m_randVec;
	int* m_permX;
	int* m_permY;
	int* m_permZ;
};
