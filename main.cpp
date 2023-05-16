#include "utils.h"

#include "color.h"
#include "camera.h"
#include "material.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"

#include <iostream>

// This code follows the Ray Tracing In a Weekend Book cited below.
//     Title (series)    : “Ray Tracing in One Weekend Series”
//     Title (book)      : “Ray Tracing in One Weekend”
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
        ray scattered;
        color attenuation;
        if (record.material_ptr->scatter(r, record.point, record.normal, attenuation, scattered)) {
            return attenuation * rayColor(scattered, entities, depth - 1);
        }
        return color(0.0, 0.0, 0.0);
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
    
    auto materialGround = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto materialCenter = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto materialLeft = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto materialRight = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    entities.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, materialGround));
    entities.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, materialCenter));
    entities.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    entities.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, materialRight));

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
