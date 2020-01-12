#pragma once

#include "join.hpp"
#include "link.hpp"
#include <vector>
#include <iostream>


struct Solver
{

	void update(float dt)
	{
		applyGravity();

		for (uint32_t i(1U); i--;) {
			for (Link& l : links) {
				l.update();
			}
		}

		for (Join& j : joins) {
			j.update();
		}

		updatePoints(dt);
		stopPoints();
	}

	void applyGravity()
	{
		for (VerletPoint::ptr pt : points) {
			pt->acceleration += Vec2(0, 1000);
		}
	}

	void updatePoints(float dt)
	{
		for (VerletPoint::ptr pt : points) {
			pt->update(dt);
		}
	}

	void stopPoints()
	{
		for (VerletPoint::ptr pt : points) {
			pt->stop();
		}
	}

	std::vector<VerletPoint::ptr> points;
	std::vector<Join> joins;
	std::vector<Link> links;
};
