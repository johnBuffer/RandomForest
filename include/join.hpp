#pragma once

#include "verlet_point.hpp"
#include "utils.hpp"


struct Join
{
	Join()
		: connected1(nullptr)
		, connected2(nullptr)
	{}

	Join(float x, float y, float angle_)
		: connected1(nullptr)
		, connected2(nullptr)
		, position(x, y)
		, angle(angle_)
	{}

	float getAngle1() const
	{
		if (!connected1) {
			return 0.0f;
		}

		const Vec2 v1 = position - connected1->position;
		const Vec2 v2 = Vec2(1.0f, 0.0f);
		return getVec2Angle(v2, v1);
	}

	float getAngle2() const
	{
		if (!connected2) {
			return 0.0f;
		}
		const Vec2 v1 = connected2->position - position;
		const Vec2 v2 = Vec2(1.0f, 0.0f);
		return getVec2Angle(v2, v);
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
