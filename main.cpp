#include "utils.h"

#include "color.h"
#include "camera.h"
#include "material.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"
#include "movingSphere.h"

#include <iostream>

// This code follows the Ray Tracing In a Weekend Book cited below.
//     Title(series) : “Ray Tracing in One Weekend Series”
//     Title(book) : “Ray Tracing : The Next Week”
//     Author : Peter Shirley
//     Editors : Steve Hollasch, Trevor David Black
//     Version / Edition : v3.2.3
//     Date : 2020 - 12 - 07
//     URL(series) : https ://raytracing.github.io/
//     URL(book) : https ://raytracing.github.io/books/RayTracingTheNextWeek.html
// ---------------------------------------------------------------------------------------
//     Author : Daniel Young
//     Version: Mar 23, 2023

hittableList randomScene() {
    hittableList entities;

    auto groundMaterial = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    entities.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            point3 center(double(a) + 0.9 * randomDouble(), 0.2, double(b) + 0.9 * randomDouble());

            if ((center - point3(4.0, 0.2, 0.0)).length() > 0.9) {
                shared_ptr<material> sphereMaterial;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphereMaterial = make_shared<lambertian>(albedo);
                    point3 endCenter = center + vec3(0.0, randomDouble(0, 0.5), 0.0);
                    entities.add(make_shared<movingSphere>(center, endCenter, 0.0, 1.0, 0.2, sphereMaterial));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<metal>(albedo, fuzz);
                    entities.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                }
                else {
                    // glass
                    sphereMaterial = make_shared<dielectric>(1.5);
                    entities.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    entities.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    entities.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    entities.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return entities;
}

color rayColor(const ray& r, const hittableList& entities, int depth) {
    hitRecord record;

    // Base Case
    if (depth <= 0) return color(0.0, 0.0, 0.0);

    if (entities.hit(r, 0.001, infinity, record)) {
        ray scattered;
        color attenuation;
        if (record.material_ptr == nullptr) {
            int q = 0;
        }
        if (record.material_ptr->scatter(r, record.point, record.normal, record.isFrontFace, attenuation, scattered)) {
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
    
    hittableList entities = randomScene();
    
    // Camera
    point3 lookFrom(13.0, 2.0, 3.0);
    point3 lookAt(0.0, 0.0, 0.0);
    vec3 vUp(0.0, 1.0, 0.0);
    double focusDistance = 10.0;
    double aperture = 0.1;
    
    camera camera(lookFrom, lookAt, vUp, 20.0, aspectRatio, aperture, focusDistance, 0.0, 1.0);

    // Render

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int row = imageHeight - 1; row >= 0; --row) {
        if (row == 207) {
            int q = 0;
        }
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
