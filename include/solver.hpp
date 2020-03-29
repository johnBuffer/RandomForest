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
		
		uint32_t debug_i = 0U;
		for (uint32_t i(1U); i--;) {
			for (Join& j : joins) {
				j.update();
			}
		}

		for (uint32_t i(1U); i--;) {
			for (Link& l : links) {
				//l.update();
			}
		}

		updatePoints(dt);
	}

	void applyGravity()
	{
		for (VerletPoint::ptr pt : points) {
			pt->applyGravity(Vec2(0, 1000));
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

	void addJoin(Join& join)
	{
		joins.push_back(join);
	}

	std::vector<VerletPoint::ptr> points;
	std::vector<Join> joins;
	std::vector<Link> links;
};
