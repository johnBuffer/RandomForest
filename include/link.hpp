#pragma once

#include "verlet_point.hpp"
#include "utils.hpp"


struct Link
{
	Link()
		: point1(nullptr)
		, point2(nullptr)
		, strength(0.1f)
	{}

	Link(VerletPoint::ptr p1, VerletPoint::ptr p2)
		: point1(p1)
		, point2(p2)
		, strength(0.75f)
	{
		length = getLength(*p1 - *p2);
	}

	void update()
	{
		const Vec2 v = *point1 - *point2;
		const Vec2 v_normalized = v.getNormalized();
		const float current_length = getLength(v);
		const float delta = length - current_length;

		point2->move(v_normalized * (-delta * strength));
	}

	VerletPoint::ptr point1;
	VerletPoint::ptr point2;
	float length;
	float strength;
};
