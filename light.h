#pragma once

#include "vec3.h"

class light {
public:
	vec3 direction = vec3(0.0, 0.0, 0.0);
	color lightColor = vec3(1.0, 1.0, 1.0);
	double intensity = 1.0;
public:
	light() {}
	light(vec3 direction, color lightColor, double intensity)
	{
		this->direction = direction;
		this->lightColor = lightColor;
		this->intensity = intensity;
	}
};