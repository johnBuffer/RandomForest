#pragma once
#include <cmath>


struct RotMat2
{
	float cosa, sina;

	RotMat2(float angle)
		: cosa(cos(angle))
		, sina(sin(angle))
	{}

	Vec2 apply(const Vec2& v) const
	{
		return Vec2(cosa * v.x - sina * v.y, sina * v.x + cosa * v.y);
	}
};


struct Vec2
{
	float x, y;

	Vec2()
		: x(0.0f)
		, y(0.0f)
	{}

	Vec2(float x_, float y_)
		: x(x_)
		, y(y_)
	{}

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
		const RotMat2 mat(angle);
		rotate(Vec2(), mat);
	}

	void rotate(const RotMat2& mat)
	{
		rotate(Vec2(), mat);
	}

	void rotate(const Vec2& origin, const RotMat2& mat)
	{
		const Vec2 v(x - origin.x, y - origin.y);
		const Vec2 rotated = mat.apply(v);
		x = rotated.x + origin.x;
		y = rotated.y + origin.y;
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

