#pragma once

#include "verlet_point.hpp"
#include "utils.hpp"


struct Join
{
	Join()
		: connected1(nullptr)
		, connected2(nullptr)
	{}

	float getAngle() const
	{
		// If no connected1, we assume connected to the ground's normal
		const Vec2 v1 = connected1 ? (position - connected1->position) : Vec2(0.0f, 1.0f);
		const Vec2 v2 = connected2 ? (connected2->position - position) : Vec2(0.0f, 1.0f);
		return getVec2Angle(v1, v2);
	}

	float getBaseAngle() const
	{
		// If no connected1, we assume connected to the ground's normal
		const Vec2 v1 = connected1 ? (position - connected1->position) : Vec2(0.0f, 1.0f);
		const Vec2 v2 = connected2 ? (connected2->position - position) : Vec2(1.0f, 0.0f);
		return getVec2Angle(v1, v2);
	}

	float getAngleDelta() const
	{
		// Difference between target angle and actual angle
		return angle - getAngle();
	}

	void update(float dt)
	{

	}

	void setConnection1(Join* join1)
	{
		connected1 = join1;
		const Vec2 v(connected1->position - position);
		length1 = sqrt(v.x*v.x + v.y*v.y);
	}

	void setConnection2(Join* join2)
	{
		connected2 = join2;
		const Vec2 v(connected2->position - position);
		length2 = sqrt(v.x*v.x + v.y*v.y);
	}

	VerletPoint position;

	float angle;
	float strength;

	Join* connected1;
	Join* connected2;

	float length1;
	float length2;
};
