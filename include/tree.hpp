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
			
		}
		else
		{
			
		}
	}
};
