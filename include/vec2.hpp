#pragma once
#include <cmath>


struct Vec2
{
	Vec2()
		: x(0.0f)
		, y(0.0f)
	{}

	Vec2(float x_, float y_)
		: x(x_)
		, y(y_)
	{}

	float x, y;

	void operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
	}

	void operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	float getLength() const
	{
		return sqrt(x*x + y*y);
	}

	Vec2 getNormalized() const
	{
		const float length = getLength();
		return Vec2(x / length, y / length);
	}

	Vec2 getNormal() const
	{
		return Vec2(-y, x);
	}

	float dot(const Vec2& v) const
	{
		return x * v.x + y * v.y;
	}

	void rotate(float angle)
	{
		rotate(Vec2(), angle);
	}

	void rotate(const Vec2& origin, float angle)
	{
		const Vec2 v(x - origin.x, y - origin.y);
		const float ca = cos(angle);
		const float sa = sin(angle);
		x = (ca * v.x - sa * v.y) + origin.x;
		y = (sa * v.x + ca * v.y) + origin.y;
	}

	float getAngle() const
	{
		const float angle = acos(x / getLength());
		return y < 0.0f ? -angle : angle;
	}

	static float getAngle(const Vec2& v1, const Vec2& v2)
	{
		const float dot = v1.x * v2.x + v1.y * v2.y;
		const float det = v1.x * v2.y - v1.y * v2.x;
		return atan2(det, dot);
	}

	static Vec2 getRotated(const Vec2& v, float angle)
	{
		Vec2 vec = v;
		vec.rotate(angle);
		return vec;
	}
};

Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x + v2.x, v1.y + v2.y);
}

Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x - v2.x, v1.y - v2.y);
}

Vec2 operator*(const Vec2& v, float f)
{
	return Vec2(v.x * f, v.y * f);
}

Vec2 operator/(const Vec2& v, float f)
{
	return Vec2(v.x / f, v.y / f);
}

