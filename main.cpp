#include <iostream>
#include <fstream>
#include <windows.h>
#include <shellapi.h>
#include <vector>
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "light.h"
#include "polygon.h"
#include "render.h"
#include "camera.h"
#include "object.h"

constexpr int ImageWidth = 512;
constexpr double AspectRatio = 1.0 / 1.0;

void setupRenderCam(camera &cam) {
    // Image
    const auto aspect_ratio = 1.0 / 1.0;
    const int imageWidth = ImageWidth;
    const int imageHeight = static_cast<int>(imageWidth / aspect_ratio);

    // Camera
    auto viewportHeight = 1.0;
    auto viewport_width = aspect_ratio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = point3(0, 0, 1);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewportHeight, 0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

    // Set values
    cam.setValues(aspect_ratio, imageWidth, viewportHeight, focalLength, origin);
}

void renderImage(std::ostream &out, const camera &cam, const render &rayTracer) {
    out << "P3\n" << cam.imageWidth << ' ' << cam.imageHeight << "\n255\n";

    // Lights
    std::vector<light> lights;
    lights.push_back(light(vec3(0.0, 1.0, 0.0), color(1.0, 1.0, 1.0), 1.0));
    rayTracer.setLights(lights);

    // Objects
    rayTracer.sceneObjects.push_back(std::make_unique<polygon>(vec3(0.0, -0.7, -0.5), vec3(1.0, 0.4, -1.0),
        vec3(0.0, -0.7, -1.5), 0.9, 1.0, 0.1, vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), 4.0, 0.0));
    rayTracer.sceneObjects.push_back(std::make_unique<polygon>(vec3(0.0, -0.7, -0.5), vec3(0.0, -0.7, -1.5),
        vec3(-1.0, 0.4, -1.0), 0.9, 1.0, 0.1, vec3(1.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0), 4.0, 0.0));

    // Render out the image
    for (int j = cam.imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < cam.imageWidth; ++i) {
            auto u = double(i) / (cam.imageWidth - 1);
            auto v = double(j) / (cam.imageHeight - 1);

            ray r(cam.origin, cam.lowerLeftCorner + u * cam.horizontal + v * cam.vertical - cam.origin);

            color defaultColor = color(0.2, 0.2, 0.2);
            color pixel_color = defaultColor;
            pixel_color = rayTracer.getPixelColor(defaultColor, r);

            write_color(out, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output_file_path>\n";
        return 1;
    }

    const std::string outputFilePath = argv[1];

    std::ofstream out(outputFilePath);
    if (!out) {
        std::cerr << "Unable to open the output file.\n";
        return 2;
    }

    camera cam;
    setupRenderCam(cam);

    render rayTracer;

    renderImage(out, cam, rayTracer);

    // Open image file after render is finished
    LPCWSTR open = L"open";
    LPCWSTR file = L"C://Users//devon//source//repos//Raytracer//Raytracer//RaytracedImage.ppm";
    ShellExecute(NULL, open, file, NULL, NULL, SW_SHOWNORMAL);

    return 0;
}
