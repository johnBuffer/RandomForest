#pragma once

#include "join.hpp"
#include <vector>


struct Solver
{

	void update(float dt)
	{
		for (Join* j_ptr : joins) {
			Join& j = *j_ptr;
			// If no extremity, nothing to update
			if (j.connected2) {
				const float delta_angle = j.getAngleDelta();
				const float target_angle = j.getBaseAngle() + (j.angle + delta_angle * (1.0f - j.strength));
				
				const float dx = cos(target_angle);
				const float dy = sin(target_angle);
				const Vec2 v = Vec2(dx, dy) * j.length2;

				j.connected2->position.moveTo(j.position.coords + v);
			}
		}
	}

	void update_pts(float dt)
	{
		for (Join* j_ptr : joins) {
			j_ptr->position.update(dt);
		}
	}


	std::vector<Join*> joins;
};
