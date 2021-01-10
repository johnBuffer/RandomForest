#pragma once
#include <vector>
#include "vec2.hpp"


struct Point
{
	Vec2 velocity;
	Vec2 position;
	bool moving;

	Point()
		: velocity(0.0f, 0.0f)
		, moving(true)
	{

	}

	void move(const Vec2& v)
	{
		position += moving * v;
	}

	void update(float dt)
	{
		const Vec2 g(0.0f, 10.0f);
		velocity += dt * g;
		position += dt * velocity;
	}
};


struct Link
{
	void update(Point& p1_, Point& p2_)
	{
		const Vec2 p2p1 = p1_.position - p2_.position;
		const float dist = p2p1.getLength();
		const Vec2 v = p2p1.getNormalized();
		
		// [0 1]
		const float strength = 0.5f;
		const float delta = length - dist;
		const Vec2 delta_v = (strength * 0.5f * delta) * v;
		p1_.move(delta_v);
		p2_.move(-1.0f * delta_v);
	}

	uint64_t p1, p2;
	float length;
};


struct Join
{
	void update(Link& l1_, Link& l2_)
	{

	}
	uint64_t l1, l2;
};


struct Solver
{
	std::vector<Point> points;
	std::vector<Link> links;
	std::vector<Join> joins;

	void update(float dt)
	{
		
	}
};
