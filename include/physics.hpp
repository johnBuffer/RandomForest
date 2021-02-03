#pragma once
#include "vec2.hpp"
#include "index_as_pointer.hpp"


struct Node
{
	// Euler data
	Vec2  position;
	Vec2  old_position;
	Vec2  acceleration;

	bool  is_static;
	float mass;

	Node()
		: position()
		, old_position()
		, is_static(false)
		, mass(1.0f)
	{}

	Node(float x, float y, float m = 1.0f)
		: position(x, y)
		, old_position(x, y)
		, is_static(false)
		, mass(m)
	{}

	void applyForce(const Vec2& force)
	{
		acceleration += force / mass;
	}

	void accelerate(const Vec2& acc)
	{
		acceleration += acc;
	}

	void integrate(float dt)
	{
		if (!is_static) {
			const Vec2 vel = position - old_position;
			old_position = position;

			position += (vel + acceleration * dt) * dt;
			acceleration = Vec2(0.0f, 0.0f);
		}
	}
};

struct Segment
{
	Index<Node> point_1;
	Index<Node> point_2;

	float length;

	Segment(Index<Node> p_1, Index<Node> p_2, float length_)
		: point_1(p_1)
		, point_2(p_2)
		, length(length_)
	{}
};

struct Joint
{
	Index<Segment> segment_1;
	Index<Segment> segment_2;

	float target;
	float strength;

	Joint(Index<Segment> s_1, Index<Segment> s_2, float target_, float strength_)
		: segment_1(s_1)
		, segment_2(s_2)
		, target(target_)
		, strength(strength_)
	{}
};
