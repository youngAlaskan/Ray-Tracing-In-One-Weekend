#include "utils.h"

#include "color.h"
#include "hittableList.h"
#include "sphere.h"

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

color rayColor(const ray& ray, const hittableList& entities) {
    hitRecord record;
    if (entities.hit(ray, 0.0, infinity, record)) return 0.5 * (record.normal + color(1.0, 1.0, 1.0));

    vec3 unitDirection = unitVector(ray.getDirection());
    double t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(double(imageWidth) / aspectRatio);

    // Entities
    hittableList entities;
    entities.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    entities.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));
    
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
            color pixelColor = rayColor(r, entities);
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone.\n";
}
