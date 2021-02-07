#pragma once
#include "index_as_ptr.hpp"
#include "node.hpp"
#include "utils.hpp"


struct Particule
{
	Vec2 position;
	Vec2 old_position;
	Vec2 acceleration;

	Particule()
		: position()
		, old_position()
		, acceleration()
	{}

	Particule(Vec2 pos)
		: position(pos)
		, old_position(pos)
		, acceleration(0.0f, 0.0f)
	{}

	void update(float dt)
	{
		const Vec2 velocity = position - old_position;
		const Vec2 new_pos = position + (velocity + acceleration * dt);
		old_position = position;
		position = new_pos;
		acceleration = Vec2(0.0f, 0.0f);
	}

	void move(Vec2 v)
	{
		position += v;
	}
};


struct PinnedSegment
{
	Index<Node> attach;
	Particule particule;
	Vec2 target_direction;
	float length;
	uint64_t branch_id;
	float last_angle;
	float delta_angle;

	PinnedSegment(Index<Node> attach_point, Vec2 free_point, uint64_t branch)
		: attach(attach_point)
		, particule(free_point)
		, target_direction((free_point - attach_point->pos).getNormalized())
		, length((free_point - attach_point->pos).getLength())
		, branch_id(branch)
		, last_angle((free_point - attach_point->pos).getAngle())
		, delta_angle(0.0f)
	{}

	void solveAttach()
	{
		const Vec2 delta = particule.position - attach->pos;
		const float dist_delta = length - delta.getLength();
		particule.move(delta.getNormalized() * dist_delta);
	}

	void update(float dt)
	{
		const float strength = 100.0f;
		particule.acceleration += target_direction * strength;
		solveAttach();
		particule.update(dt);
		updateAngle();
	}

	void updateAngle()
	{
		const float angle = (particule.position - attach->pos).getAngle();
		delta_angle = angle - last_angle;
		last_angle = angle;
	}
};
