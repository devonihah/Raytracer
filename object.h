#pragma once

#include "matAttributes.h"

class object {
public:
	enum class objectType { SPHERE, POLYGON };
	matAttributes attribs;
	objectType type = objectType::POLYGON;
public:
	object() {}
};
