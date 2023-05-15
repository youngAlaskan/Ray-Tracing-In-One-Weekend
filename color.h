#pragma once

#include "vec3.h"

#include <iostream>

void writeColor(std::ostream& out, color pixelColor) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixelColor.x()) << ' '
        << static_cast<int>(255.999 * pixelColor.y()) << ' '
        << static_cast<int>(255.999 * pixelColor.z()) << '\n';
}

void writeColor(std::ostream& out, color pixelColor, int samplesPerPixel) {
    double red = pixelColor.x();
    double green = pixelColor.y();
    double blue = pixelColor.z();

    // Divide the color by the number of smaples and gamma-correct for gamma = 2.0.
    double scale = 1.0 / samplesPerPixel;
    red = sqrt(scale * red);
    green = sqrt(scale * green);
    blue = sqrt(scale * blue);

    // Write the translated [0, 255] value of each color component.
    out << static_cast<int>(256 * clamp(red, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(green, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(blue, 0.0, 0.999)) << '\n';
}