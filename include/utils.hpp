#pragma once

#include "vec2.hpp"
#include <cmath>
#include <iostream>


const float PI = 3.141592653f;


float getVec2Angle(const Vec2& v1, const Vec2& v2)
{
	const float dot = v1.x * v2.x + v1.y * v2.y;
	const float det = v1.x * v2.y - v1.y * v2.x;
	return atan2(det, dot);
}


float getRandRange(float width)
{
	constexpr int32_t max_value = 10000;
	const float val = (rand() % max_value - (max_value>>1)) / float(max_value) * width;
	std::cout << "Range width: " << width << " generated: " << val << std::endl;
	return val;
}


float getRandUnder(float max)
{
	constexpr uint32_t max_value = 10000;
	const float val = (rand() % max_value) / float(max_value) * max;
	return val;
}


float getLength(const Vec2& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}


Vec2 rotate(const Vec2& v, float angle, const Vec2& origin = Vec2(0.0f, 0.0f))
{
	const Vec2 origin_centered = v - origin;
	const float new_x = origin_centered.x * cos(angle) - origin_centered.y * sin(angle) + origin.x;
	const float new_y = origin_centered.x * sin(angle) + origin_centered.y * cos(angle) + origin.y;

	return Vec2(new_x, new_y);
}
