#pragma once

#include "vec2.hpp"
#include <cmath>

struct Utils
{

	const float PI = 3.141592653f;

	static float getLength(const sf::Vector2f& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	static sf::Vector2f getNormalized(const sf::Vector2f& v)
	{
		return v / getLength(v);
	}

	static float getVec2Angle(const Vec2& v1, const Vec2& v2)
	{
		const float dot = v1.x * v2.x + v1.y * v2.y;
		const float det = v1.x * v2.y - v1.y * v2.x;
		return atan2(det, dot);
	}

	static float getRandRange(float width)
	{
		constexpr int32_t max_value = 10000;
		return (rand() % max_value - max_value / 2) / float(max_value) * width;
	}

	static float getRandUnder(float max)
	{
		constexpr int32_t max_value = 10000;
		return (rand() % max_value) / float(max_value) * max;
	}

	static float getLength(const Vec2& v)
	{
		return sqrt(v.x*v.x + v.y*v.y);
	}

	static Vec2 rotate(const Vec2& pt, const Vec2& origin, float angle)
	{
		const Vec2 v = pt - origin;

		const float ca = cos(angle);
		const float sa = sin(angle);

		const float x = v.x * ca - v.y * sa;
		const float y = v.x * sa + v.y * ca;

		return Vec2(x, y) + origin;
	}

	static float sign(float a)
	{
		return a < 0.0f ? -1.0f : 1.0f;
	}

};

