#pragma once
#include "vec2.hpp"
#include "number_generator.hpp"


struct Leaf
{
	Vec2 attach;
	Vec2 position;
	Vec2 old_position;
	Vec2 target_direction;
	Vec2 acceleration;
	float joint_strenght;

	Leaf(const Vec2& pos, const Vec2& dir)
		: attach(pos)
		, position(pos + dir)
		, old_position(position)
		, target_direction(dir)
		, joint_strenght(RNGf::getRange(1.0f, 2.0f))
	{}

	void solveAttach()
	{
		const float length = 1.0f;
		const Vec2 delta = position - attach;
		const float dist_delta = 1.0f - delta.getLength();
		position = position + delta.getNormalized() * dist_delta;
	}

	Vec2 getDir() const
	{
		return position - attach;
	}

	void update(float dt)
	{
		solveAttach();
		const Vec2 velocity = position - old_position;
		const Vec2 new_pos = position + (velocity + acceleration * dt);
		old_position = position;
		position = new_pos;
		acceleration = target_direction * joint_strenght;
	}
};
