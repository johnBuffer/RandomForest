#pragma once

#include "vec2.hpp"
#include <memory>


struct VerletPoint
{
	using ptr = std::shared_ptr<VerletPoint>;

	// All to 0.0f
	VerletPoint()
		: coords()
		, last_position()
		, acceleration()
		, moving(true)
	{}

	VerletPoint(float x, float y)
		: coords(x, y)
		, last_position(x, y)
		, acceleration()
		, moving(true)
	{}

	void moveTo(float x, float y)
	{
		if (moving) {
			coords.x = x;
			coords.y = y;
		}
	}

	void moveTo(const Vec2& p)
	{
		if (moving) {
			coords.x = p.x;
			coords.y = p.y;
		}
	}

	void move(const Vec2& p)
	{
		if (moving) {
			coords.x += p.x;
			coords.y += p.y;
		}
	}

	void update(float dt)
	{
		if (moving) {
			const Vec2 v = coords - last_position;
			last_position = coords;
			coords += v + acceleration * (dt * dt);
			acceleration = Vec2(0.0f, 0.0f);
		}
	}

	static ptr create(float x, float y)
	{
		return std::make_shared<VerletPoint>(x, y);
	}

	Vec2 coords;
	Vec2 last_position;
	Vec2 acceleration;

	bool moving;
};


Vec2 operator-(const VerletPoint& p1, const VerletPoint& p2)
{
	return p1.coords - p2.coords;
}
