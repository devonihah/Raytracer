#pragma once

#include "object.h"
#include "vec3.h"
#include "matAttributes.h"

class polygon : public object{
public:
	vec3 p1;
	vec3 p2;
	vec3 p3;

public:
	polygon(vec3 p1, vec3 p2, vec3 p3, double kd = 0.0, double ks = 0.0, double ka = 0.0,
		vec3 od = vec3(1.0, 1.0, 1.0), vec3 os = vec3(1.0, 1.0, 1.0), double kgls = 1.0,
		double refl = 0.0, objectType type = objectType::POLYGON)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		this->attribs.od = od;
		this->attribs.os = os;
		this->attribs.kd = kd;
		this->attribs.ks = ks;
		this->attribs.ka = ka;
		this->attribs.kgls = kgls;
		this->attribs.refl = refl;
		this->type = type;
	}

	bool operator!=(const polygon p) {
		if (p1 != p.p1 
			|| p2 != p.p2 
			|| p3 != p.p3 
			|| attribs.od != p.attribs.od 
			|| attribs.os != p.attribs.os
			|| attribs.kd != p.attribs.kd
			|| attribs.ks != p.attribs.ks
			|| attribs.ka != p.attribs.ka
			|| attribs.kgls != p.attribs.kgls
			|| attribs.refl != p.attribs.refl
			|| type != p.type) return true;
		return false;
	}

	bool operator==(const polygon p) {
		if (p1 == p.p1
			&& p2 == p.p2
			&& p3 == p.p3
			&& attribs.od == p.attribs.od
			&& attribs.os == p.attribs.os
			&& attribs.kd == p.attribs.kd
			&& attribs.ks == p.attribs.ks
			&& attribs.ka == p.attribs.ka
			&& attribs.kgls == p.attribs.kgls
			&& attribs.refl == p.attribs.refl
			&& type == p.type) return true;
		return false;
	}
};
