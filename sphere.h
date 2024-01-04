#pragma once

#include "object.h"
#include "vec3.h"
#include "matAttributes.h"

class sphere : public object{
public:
	vec3 center;
	vec3 od;
	vec3 os;
	double radius = 0.0;

public:
	sphere() {}
	sphere(vec3 center, double radius, double kd = 0.0, double ks = 0.0, double ka = 0.0, 
		vec3 od = vec3(1.0, 1.0, 1.0), vec3 os = vec3(1.0, 1.0, 1.0), double kgls = 1.0, 
		double refl = 0.0, objectType type = objectType::SPHERE)
	{
		this->center = center;
		attribs.od = od;
		attribs.os = os;
		this->radius = radius;
		attribs.kd = kd;
		attribs.ks = ks;
		attribs.ka = ka;
		attribs.kgls = kgls;
		attribs.refl = refl;
		this->type = type;
	}

	bool operator!=(const sphere s) {
		if (center != s.center
			|| attribs.od != s.attribs.od
			|| attribs.os != s.attribs.os
			|| radius != s.radius
			|| attribs.kd != s.attribs.kd
			|| attribs.ks != s.attribs.ks
			|| attribs.ka != s.attribs.ka
			|| attribs.kgls != s.attribs.kgls
			|| attribs.refl != s.attribs.refl
			|| type != s.type) return true;
		return false;
	}

	bool operator==(const sphere s) {
		if (center == s.center
			&& attribs.od == s.attribs.od
			&& attribs.os == s.attribs.os
			&& radius == s.radius
			&& attribs.kd == s.attribs.kd
			&& attribs.ks == s.attribs.ks
			&& attribs.ka == s.attribs.ka
			&& attribs.kgls == s.attribs.kgls
			&& attribs.refl == s.attribs.refl
			&& type == s.type) return true;
		return false;
	}
};