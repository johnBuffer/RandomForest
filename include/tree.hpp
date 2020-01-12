#pragma once

#include "solver.hpp"


struct Branch
{
	float length;
	float angle_with_parent;
};


struct Tree
{
	float fork_angle;
	float fork_amplitude;
	
	float branch_length;
	float branch_length_ratio;
	float branch_width;
	float branch_width_ratio;

	void generate(Solver& solver, Join* current_join, uint32_t stage_count)
	{
		if (!stage_count) {
			return;
		}

		if (!current_join) {
			Join* root = new Join(800, 900, -PI * 0.5f);
			solver.joins.push_back(root);
			generate(solver, root, stage_count - 1);
		}
		else
		{
			const Vec2& parent_position = current_join->position.coords;
			Join* new_join = new Join(800, 900, -PI * 0.5f);
			solver.joins.push_back(root);
			generate(solver, root, stage_count - 1);
		}
	}
};
