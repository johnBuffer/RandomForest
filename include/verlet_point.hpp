#pragma once

#include "vec2.hpp"


struct VerletPoint
{
	// All to 0.0f
	VerletPoint()
		: coords()
		, last_position()
		, acceleration()
	{}

	VerletPoint(float x, float y)
		: coords(x, y)
		, last_position(x, y)
		, acceleration()
	{}

	void moveTo(float x, float y)
	{
		coords.x = x;
		coords.y = y;
	}

	void moveTo(const Vec2& p)
	{
		coords.x = p.x;
		coords.y = p.y;
	}

	void update(float dt)
	{
		const Vec2 v = coords - last_position;
		last_position = coords;
		coords += v + acceleration * (dt * dt);
	}

	Vec2 coords;
	Vec2 last_position;
	Vec2 acceleration;
};


Vec2 operator-(const VerletPoint& p1, const VerletPoint& p2)
{
	return p1.coords - p2.coords;
}
