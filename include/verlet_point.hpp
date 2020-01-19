#pragma once

#include "vec2.hpp"
#include <memory>


struct VerletPoint
{
	using ptr = std::shared_ptr<VerletPoint>;

	// All to 0.0f
	VerletPoint()
		: coords()
		, last_coords()
		, acceleration()
		, mass(1.0f)
		, mass_coef(1.0f)
		, moving(true)
	{}

	VerletPoint(float x, float y, float mass_)
		: coords(x, y)
		, last_coords(x, y)
		, acceleration()
		, mass(mass_)
		, mass_coef(1.0f / mass_)
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

	void stop()
	{
		last_coords = coords;
	}

	void update(float dt)
	{
		if (moving) {
			const Vec2 v = coords - last_coords;
			acceleration -=  v * 1000.0f;
			last_coords = coords;
			coords += v + acceleration * (dt * dt);
			acceleration = Vec2(0.0f, 0.0f);
		}
	}

	void applyGravity(const Vec2& gravity)
	{
		acceleration += gravity;
	}

	void applyForce(const Vec2& force)
	{
		acceleration += force * mass_coef;
	}

	static ptr create(float x, float y, float mass)
	{
		return std::make_shared<VerletPoint>(x, y, mass);
	}

	void reverse()
	{
		coords = last_coords;
	}

	Vec2 coords;
	Vec2 last_coords;
	Vec2 acceleration;

	const float mass;
	const float mass_coef;
	bool moving;
};


Vec2 operator-(const VerletPoint& p1, const VerletPoint& p2)
{
	return p1.coords - p2.coords;
}
