#include "utils.h"

#include "color.h"
#include "camera.h"
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

color rayColor(const ray& r, const hittableList& entities, int depth) {
    hitRecord record;

    // Base Case
    if (depth <= 0) return color(0.0, 0.0, 0.0);

    if (entities.hit(r, 0.001, infinity, record)) {
        point3 target = record.point + record.normal + randomUnitVector();
        return 0.5 * rayColor(ray(record.point, target - record.point), entities, depth - 1);
    }

    vec3 unitDirection = unitVector(r.getDirection());
    double t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(double(imageWidth) / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    // Entities
    hittableList entities;
    entities.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    entities.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));
    
    // Camera
    camera camera;

    // Render

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int row = imageHeight - 1; row >= 0; --row) {
        std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;
        for (int column = 0; column < imageWidth; ++column) {
            color pixelColor(0.0, 0.0, 0.0);
            for (int s = 0; s < samplesPerPixel; s++) {
                double u = (double(column) + randomDouble()) / (imageWidth - 1);
                double v = (double(row) + randomDouble()) / (imageHeight - 1);
                ray r = camera.getRay(u, v);
                pixelColor += rayColor(r, entities, maxDepth);
            }
            writeColor(std::cout, pixelColor, samplesPerPixel);
        }
    }

    std::cerr << "\nDone.\n";
}
