#pragma once

#include "vec2.hpp"
#include <cmath>


float getVec2Angle(const Vec2& v1, const Vec2& v2)
{
	const float dot = v1.x * v2.x + v1.y * v2.y;
	const float det = v1.x * v2.y - v1.y * v2.x; // x1 * y2 - y1 * x2      # determinant
	return atan2(det, dot);
}
