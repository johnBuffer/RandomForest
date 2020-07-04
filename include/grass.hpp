#pragma once

#include "solver.hpp"

struct Grass
{
	static void add(Solver& solver, float x, float y)
	{
		float strength = 0.02f;
		float link_length = 20.0f + rand() % 10;
		const uint32_t points_count = 3;
		VerletPoint::ptr last_point = solver.createPoint(x, y, 1.0f, false);
		Link::ptr last_link = nullptr;
		for (uint32_t i(0); i < points_count; ++i) {
			VerletPoint::ptr new_point = solver.createPoint(last_point->coords.x, last_point->coords.y - link_length, 0.02f / float(i + 1), i > 0);
			Link::ptr new_link = solver.createLink(last_point, new_point);
			last_point = new_point;
			if (i > 0 && i < 40) {
				solver.createJoin(last_link, new_link, 0.0f, strength);
				strength *= 5.0f;
			}
			link_length *= 0.7f;
			last_link = new_link;
		}
	}
};

