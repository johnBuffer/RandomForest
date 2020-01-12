#pragma once

#include "join.hpp"
#include <vector>
#include <iostream>


struct Solver
{

	void update(float dt)
	{
		for (Join* j_ptr : joins) {
			Join& j = *j_ptr;
			// If no extremity, nothing to update
			if (j.connected2) {
				const float angle1 = j.getAngle1();
				const float angle2 = j.getAngle2();
				const float current_angle = angle2 - angle1;
				const float delta = j.angle - current_angle;
				const float target_angle = angle2 + delta * j.strength;

				const float dx = cos(target_angle);
				const float dy = sin(target_angle);
				const Vec2 v = Vec2(dx, dy) * j.length2;

				j.connected2->position.moveTo(j.position.coords + v);
			}
		}

		update_pts(dt);
	}

	void update_pts(float dt)
	{
		for (Join* j_ptr : joins) {
			j_ptr->position.update(dt);
		}
	}

	std::vector<Join*> joins;
};
