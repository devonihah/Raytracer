#pragma once

#include "vec3.h"

class camera {
public:
	double aspectRatio = 1.0 / 1.0;
	int imageWidth = 512;
	int imageHeight = 512;

	double viewportHeight = 1.0;
	double viewportWidth = 1.0;
	double focalLength = 2.0;

	vec3 origin = point3(0, 0, 1);
	vec3 horizontal = vec3(1, 0, 0);
	vec3 vertical = vec3(0, 1, 0);
	vec3 lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

public:
	camera() {}

	camera(double aspectRatio, int imageWidth, double viewportHeight,
		double focalLength, vec3 origin = point3(0,0,1))
	{
		this->aspectRatio = aspectRatio;
		this->imageWidth = imageWidth;
		this->imageHeight = static_cast<int>(imageWidth / aspectRatio);
		this->viewportHeight = viewportHeight;
		this->viewportWidth = aspectRatio * viewportHeight;
		this->focalLength = focalLength;
		this->origin = origin;
		vec3 horizontal = vec3(viewportWidth, 0, 0);
		vec3 vertical = vec3(0, viewportHeight, 0);
		vec3 lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);
	}

	void setValues(double aspectRatio, int imageWidth, double viewportHeight,
		double focalLength, vec3 origin = point3(0, 0, 1))
	{
		this->aspectRatio = aspectRatio;
		this->imageWidth = imageWidth;
		this->imageHeight = static_cast<int>(imageWidth / aspectRatio);
		this->viewportHeight = viewportHeight;
		this->viewportWidth = aspectRatio * viewportHeight;
		this->focalLength = focalLength;
		this->origin = origin;
		this->horizontal = vec3(viewportWidth, 0, 0);
		this->vertical = vec3(0, viewportHeight, 0);
		this->lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);
	}
};
