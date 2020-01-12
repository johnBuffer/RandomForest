#pragma once

#include "solver.hpp"


struct Branch
{
	float length;
	VerletPoint::ptr extremity;
	VerletPoint::ptr base;
};


struct Tree
{
	uint32_t fork_count;
	float fork_angle;
	float fork_amplitude;

	float branch_length;
	float branch_length_ratio;
	float branch_width;
	float branch_width_ratio;

	void create(Solver& solver, uint32_t stage_count, float x, float y)
	{
		VerletPoint::ptr root = VerletPoint::create(x, y, 10.0f);
		root->moving = false;

		solver.points.push_back(root);

		Branch branch = createBranch(solver, root);

		generate(solver, branch, stage_count);
	}

	void generate(Solver& solver, Branch last_branch, uint32_t stage_count)
	{
		if (!stage_count) {
			return;
		}

		uint32_t forks = 1U + rand() % fork_count;
		for (uint32_t i(forks); i--;) {
			float new_length = last_branch.length * branch_length_ratio;
			float angle = getRandRange(fork_amplitude);
			VerletPoint::ptr last_base = last_branch.base;
			VerletPoint::ptr last_point = last_branch.extremity;
			VerletPoint::ptr new_point = VerletPoint::create(last_point->coords.x + rand()%10, last_point->coords.y - new_length, stage_count + 1);

			solver.joins.push_back(Join(last_base, last_point, new_point, angle, new_length, 0.05f));
			solver.links.push_back(Link(last_point, new_point));
			solver.points.push_back(new_point);

			Branch branch;
			branch.length = new_length;
			branch.base = last_point;
			branch.extremity = new_point;

			generate(solver, branch, stage_count - 1);
		}
	}

	Branch createBranch(Solver& solver, VerletPoint::ptr root)
	{
		float angle = -0.5f * PI;
		VerletPoint::ptr last_base = nullptr;
		VerletPoint::ptr last_point = root;
		VerletPoint::ptr new_point = VerletPoint::create(last_point->coords.x, last_point->coords.y - branch_length, 10.0f);

		solver.joins.push_back(Join(last_base, last_point, new_point, angle, branch_length, 0.05f));
		solver.links.push_back(Link(last_point, new_point));
		solver.points.push_back(new_point);

		Branch branch;
		branch.length = branch_length;
		branch.base = root;
		branch.extremity = new_point;

		return branch;
	}
};
