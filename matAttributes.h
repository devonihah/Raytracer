#pragma once

#include "vec3.h"

struct matAttributes {
public:
	vec3 od;
	vec3 os;
	double kd = 0.0;
	double ks = 0.0;
	double ka = 0.0;
	double kgls = 0.0;
	double refl = 0.0;
public:
	matAttributes() {}
	matAttributes(double kd, double ks, double ka, vec3 od, vec3 os, double kgls, double refl)
	{
		this->kd = kd;
		this->ks = ks;
		this->ka = ka;
		this->od = od;
		this->os = os;
		this->kgls = kgls;
		this->refl = refl;
	}

};
