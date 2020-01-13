#pragma once

#include "verlet_point.hpp"
#include "utils.hpp"


struct Link
{
	Link()
		: point1(nullptr)
		, point2(nullptr)
		, strength(1.0f)
	{}

	Link(VerletPoint::ptr p1, VerletPoint::ptr p2)
		: point1(p1)
		, point2(p2)
		, strength(1.0f)
	{
		length = getLength(*p1 - *p2);
	}

	void update()
	{
		const Vec2 v = *point1 - *point2;
		const Vec2 v_normalized = v.getNormalized();
		const float current_length = getLength(v);
		const float delta = length - current_length;
		const float amp = 0.5f * delta * strength;

		const float mass_total = point1->mass + point2->mass;
		const float mass_coef1 = point2->mass / mass_total;
		const float mass_coef2 = point1->mass / mass_total;
		//const float coef = (!point1->moving || !point2->moving) ? 1.0f : 0.5f;
		point1->move(v_normalized *  mass_coef1 * amp);
		point2->move(v_normalized * -mass_coef1 * amp);
	}

	VerletPoint::ptr point1;
	VerletPoint::ptr point2;
	float length;
	float strength;
};
