#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// This code follows the Ray Tracing In a Weekend Book cited below.
//     Title (series)    : “Ray Tracing in One Weekend Series”
//     Title(book)       : “Ray Tracing in One Weekend”
//     Author            : Peter Shirley
//     Editors           : Steve Hollasch, Trevor David Black
//     Version / Edition : v3.2.3
//     Date              : 2020 - 12 - 07
//     URL(series)       : https://raytracing.github.io/
//     URL(book)         : https://raytracing.github.io/books/RayTracingInOneWeekend.html
// ---------------------------------------------------------------------------------------
//     Author : Daniel Young
//     Version: Mar 23, 2023

bool hitSphere(const point3& center, double radius, const ray& r) {
    // Equation for the intersection of a sphere and a ray:
    //     t^2 * b^2 + 2 * t * b * (a - c) + (a - c)^2 - r^2 = 0
    // Where we have a...
    //   - ray   : a + t * b
    //   - center: c
    //   - radius: r
    // to solve we use the quadratic formula.

    vec3 oc = r.getOrigin() - center;
    double a = dot(r.getDirection(), r.getDirection());
    double b = 2.0 * dot(oc, r.getDirection());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

color rayColor(const ray& r) {
    if (hitSphere(point3(0.0, 0.0, -1.0), 0.5, r))
        return color(1.0, 0.0, 0.0);
    vec3 unitDirection = unitVector(r.getDirection());
    double t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(double(imageWidth) / aspectRatio);

    // Camera

    double viewportHeight = 2.0;
    double viewportWidth = aspectRatio * viewportHeight;
    double focalLength = 1.0;

    point3 origin = point3(0.0, 0.0, 0.0);
    vec3 horizontal = vec3(viewportWidth, 0.0, 0.0);
    vec3 vertical = vec3(0.0, viewportHeight, 0.0);
    vec3 lowerLeftCorner = origin - (horizontal + vertical) * 0.5 - vec3(0, 0, focalLength);

    // Render

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int row = imageHeight - 1; row >= 0; --row) {
        std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;
        for (int column = 0; column < imageWidth; ++column) {
            double u = double(column) / (imageWidth - 1);
            double v = double(row) / (imageHeight - 1);
            ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
            color pixelColor = rayColor(r);
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone.\n";
}
