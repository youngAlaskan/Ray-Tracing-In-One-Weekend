#include "utils.h"

#include "color.h"
#include "camera.h"
#include "material.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"
#include "movingSphere.h"
#include "tm.h"

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
//     Version: Apr 9, 2023

hittableList randomScene() {
    hittableList entities;

    auto checker = make_shared<checkerTexture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    entities.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, make_shared<lambertian>(checker)));

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

hittableList twoSpheres() {
    hittableList entities;

    auto checker = make_shared<checkerTexture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    entities.add(make_shared<sphere>(point3(0.0, -10.0, 0.0), 10, make_shared<lambertian>(checker)));
    entities.add(make_shared<sphere>(point3(0.0, 10.0, 0.0), 10, make_shared<lambertian>(checker)));

    return entities;
}

hittableList twoPerlinSpheres() {
    hittableList entities;

    auto perlinTex = make_shared<perlinTexture>(4.0);
    entities.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000, make_shared<lambertian>(perlinTex)));
    entities.add(make_shared<sphere>(point3(0.0, 2.0, 0.0), 2, make_shared<lambertian>(perlinTex)));

    return entities;
}

hittableList earth() {
    auto earthTex = make_shared<imageTexture>("earthmap.jpg");
    auto earthSurface = make_shared<lambertian>(earthTex);
    auto globe = make_shared<sphere>(point3(0.0, 0.0, 0.0), 2, earthSurface);

    return hittableList(globe);
}

hittableList simpleLight() {
    hittableList entities;

    auto perlinTex = make_shared<perlinTexture>(4);
    entities.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000, make_shared<lambertian>(perlinTex)));
    entities.add(make_shared<sphere>(point3(0.0, 2.0, 0.0), 2, make_shared<lambertian>(perlinTex)));

    auto difflight = make_shared<diffuseLight>(color(4.0, 4.0, 4.0));
    entities.add(make_shared<aaRectangle>(3, 5, 1, 3, -2, -2, difflight));
    //entities.add(make_shared<sphere>(point3(0.0, 8.0, 0.0), 2, difflight));

    return entities;
}

hittableList cornellBox() {
    hittableList entities;

    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuseLight>(color(15.0, 15.0, 15.0));

    //entities.add(make_shared<aaRectangle>(555.0, 555.0, 0.0, 555.0, 0.0, 555.0, green));
    //entities.add(make_shared<aaRectangle>(0.0, 0.0, 0.0, 555.0, 0.0, 555.0, red));
    //entities.add(make_shared<aaRectangle>(213.0, 343, 554.0, 554.0, 227.0, 332, light));
    //entities.add(make_shared<aaRectangle>(0.0, 555.0, 0.0, 0.0, 0.0, 555.0, white));
    //entities.add(make_shared<aaRectangle>(0.0, 555.0, 555.0, 555.0, 0.0, 555.0, white));
    //entities.add(make_shared<aaRectangle>(0.0, 555.0, 0.0, 555.0, 555.0, 555.0, white));

    return entities;
}

color rayColor(const ray& r, const color& background, const hittableList& entities, int depth) {
    hitRecord record;

    // Base Case
    if (depth <= 0) return color(0.0, 0.0, 0.0);

    // If the ray hits nothing, return the background color.
    if (!entities.hit(r, 0.001, infinity, record))
        return background;

    ray scattered;
    color attenuation;
    color emitted = record.material_ptr->emitted(record.u, record.v, record.point);

    if (!record.material_ptr->scatter(r, record.point, record.normal,
        record.isFrontFace, record.u, record.v, attenuation, scattered))
    { return emitted; }

    return emitted + attenuation * rayColor(scattered, background, entities, depth - 1);
}

int main() {

    // Image

    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;
    int samplesPerPixel = 100;
    const int maxDepth = 50;

    // Entities
    
    hittableList entities;

    point3 lookFrom;
    point3 lookAt;
    double vFOV = 0.0;
    double aperture = 0.0;
    color background(0.0, 0.0, 0.0);

    switch (6) {
    case 1:
        entities = randomScene();
        background = color(0.70, 0.80, 1.00);
        lookFrom = point3(13.0, 2.0, 3.0);
        lookAt = point3(0.0, 0.0, 0.0);
        vFOV = 20.0;
        aperture = 0.1;
        break;
    case 2:
        entities = twoSpheres();
        background = color(0.70, 0.80, 1.00);
        lookFrom = point3(13.0, 2.0, 3.0);
        lookAt = point3(0.0, 0.0, 0.0);
        vFOV = 20.0;
        break;
    case 3:
        entities = twoPerlinSpheres();
        background = color(0.70, 0.80, 1.00);
        lookFrom = point3(13.0, 2.0, 3.0);
        lookAt = point3(0.0, 0.0, 0.0);
        vFOV = 20.0;
        break;
    case 4:
        entities = earth();
        background = color(0.70, 0.80, 1.00);
        lookFrom = point3(13.0, 2.0, 3.0);
        lookAt = point3(0.0, 0.0, 0.0);
        vFOV = 20.0;
        break;
    case 5:
        entities = simpleLight();
        samplesPerPixel = 400;
        background = color(0.0, 0.0, 0.0);
        lookFrom = point3(26.0, 3.0, 6.0);
        lookAt = point3(0.0, 2.0, 0.0);
        vFOV = 20.0;
        break;
    case 6:
        entities = cornellBox();
        aspectRatio = 1.0;
        imageWidth = 600;
        samplesPerPixel = 200;
        background = color(0.0, 0.0, 0.0);
        lookFrom = point3(278.0, 278.0, -800.0);
        lookAt = point3(278.0, 278.0, 0.0);
        vFOV = 40.0;
        break;
    default:
        background = color(0.0, 0.0, 0.0);
        break;
    }
    
    // Camera

    vec3 vUp(0.0, 1.0, 0.0);
    double focusDistance = 10.0;
    int imageHeight = static_cast<int>(double(imageWidth) / aspectRatio);

    camera camera(lookFrom, lookAt, vUp, vFOV, aspectRatio, aperture, focusDistance, 0.0, 1.0);

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
                pixelColor += rayColor(r, background, entities, maxDepth);
            }
            writeColor(std::cout, pixelColor, samplesPerPixel);
        }
    }

    std::cerr << "\nDone.\n";
}
