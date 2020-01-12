#pragma once

#include "vec2.hpp"
#include <cmath>

const float PI = 3.141592653;


float getVec2Angle(const Vec2& v1, const Vec2& v2)
{
	const float dot = v1.x * v2.x + v1.y * v2.y;
	const float det = v1.x * v2.y - v1.y * v2.x;
	return atan2(det, dot);
}

float getRandRange(float width)
{
	return (rand() % 1000 - 500) / 1000.0f * width;
}
