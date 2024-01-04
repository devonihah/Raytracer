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

void setupRenderCam(camera &cam)
{
	//Image
	const auto aspect_ratio = 1.0 / 1.0;
	const int imageWidth = 512;
	const int imageHeight = static_cast<int>(imageWidth / aspect_ratio);

	//Camera
	auto viewportHeight = 1.0;
	auto viewport_width = aspect_ratio * viewportHeight;
	auto focalLength = 1.0;

	auto origin = point3(0, 0, 1);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewportHeight, 0);
	auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

	//Set values
	cam.setValues(aspect_ratio, imageWidth, viewportHeight, focalLength, origin);
}

int main(int argc, char *argv[])
{
//Output file
	std::ofstream out(argv[1]);                                      // Check if out.txt file opens
	if (!out)
	{
		std::cerr << "Unable to open the output file.";
		return 2;
	}

//Camera
	camera cam;
	setupRenderCam(cam);

//Renderer
	render rayTracer;

	out << "P3\n" << cam.imageWidth << ' ' << cam.imageHeight << "\n255\n";

//Lights
	std::vector<light> lights;
	lights.push_back(light(vec3(0.0, 1.0, 0.0), color(1.0, 1.0, 1.0), 1.0));
	rayTracer.setLights(lights);

//Objects
	///*

	//Polygons
	rayTracer.sceneObjects.push_back(std::make_unique<polygon>(vec3(0.0, -0.7, -0.5), vec3(1.0, 0.4, -1.0),
		vec3(0.0, -0.7, -1.5), 0.9, 1.0, 0.1, vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), 4.0, 0.0));
	rayTracer.sceneObjects.push_back(std::make_unique<polygon>(vec3(0.0, -0.7, -0.5), vec3(0.0, -0.7, -1.5),
		vec3(-1.0, 0.4, -1.0), 0.9, 1.0, 0.1, vec3(1.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0), 4.0, 0.0));

//Render out the image
	for (int j = cam.imageHeight - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

		for (int i = 0; i < cam.imageWidth; ++i) {
			auto u = double(i) / (cam.imageWidth - 1);
			auto v = double(j) / (cam.imageHeight - 1);

			ray r(cam.origin, cam.lowerLeftCorner + u * cam.horizontal + v * cam.vertical - cam.origin);

			color defaultColor = color(0.2, 0.2, 0.2);
			color pixel_color = defaultColor;
			pixel_color = rayTracer.getPixelColor(defaultColor, r);
			/*
			for (int k = 0; k < rayTracer.sceneObjects.size(); ++k)
			{
				pixel_color = rayTracer.ray_color(r, *static_cast<polygon*>(rayTracer.sceneObjects.at(k).get()));
				if (pixel_color == defaultColor)
				{
					pixel_color = rayTracer.ray_color(r, *static_cast<sphere*>(rayTracer.sceneObjects.at(k).get()));
				}
				
				if (pixel_color != defaultColor) break;
			}
			*/
			
			write_color(out, pixel_color);
		}
	}

    std::cerr << "\nDone.\n";

//Open image file after render is finished
	LPCWSTR open = L"open";
	LPCWSTR file = L"C://Users//devon//source//repos//Raytracer//Raytracer//RaytracedImage.ppm";
	ShellExecute(NULL, open, file, NULL, NULL, SW_SHOWNORMAL);

	return 0;
}