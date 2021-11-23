#pragma once
#include <glm/glm.hpp>
using namespace glm;
struct Bound2 {
	vec2 max;
	vec2 min;
	Bound2(float xMax, float yMax, float xMin, float yMin)
		:max(xMax, yMax), min(xMin, yMin)
	{

	}
};