#pragma once

#include <cmath>
#include <random>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

inline double randomDouble() {
	// C++ style random generation
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
	
	// C style random generation
	// return std::rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
	return min + (max - min) * randomDouble();
}

inline int randomInt(int min, int max) {
	return static_cast<int>(randomDouble(min, max + 1));
}

inline double clamp(double val, double min, double max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

// Common Headers

#include "ray.h"
#include "vec3.h"
