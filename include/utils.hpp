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
	constexpr uint32_t max_value = 10000;
	return (rand() % max_value - max_value/2U) / float(max_value) * width;
}

float getRandUnder(float max)
{
	constexpr uint32_t max_value = 10000;
	return (rand() % max_value) / float(max_value) * max;
}


float getLength(const Vec2& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}
