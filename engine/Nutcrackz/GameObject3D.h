#pragma once

#include <string>

#include "Vector3.h"

struct GameObject3D
{
	std::string name;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};