#pragma once

#include "solver.hpp"


struct Branch
{
	float join_strength;
	float length;
	VerletPoint::ptr extremity;
	VerletPoint::ptr base;

	void rotate(VerletPoint::ptr new_extremity)
	{
		base = extremity;
		extremity = new_extremity;
	}
};


struct Tree
{
	float fork_probability;
	float fork_amplitude;

	float branch_length;
	float branch_length_ratio;
	float branch_width;
	float branch_width_ratio;
	float branch_distortion;

	void create(Solver& solver, uint32_t stage_count, float x, float y)
	{
		VerletPoint::ptr root = VerletPoint::create(x, y, 10.0f);
		root->moving = false;

		solver.points.push_back(root);

		Branch branch = createBranch(solver, root);

		generate(solver, branch, 0U, stage_count);
	}

	void generate(Solver& solver, Branch last_branch, uint32_t stage_count, uint32_t max_stage)
	{
		if (stage_count >= max_stage) {
			return;
		}

		VerletPoint::ptr last_base = last_branch.base;
		VerletPoint::ptr last_point = last_branch.extremity;

		const float fork_rand = getRandUnder(1.0f);
		// Here we fork
		if (fork_rand < fork_probability) {
			Branch branch = last_branch;
			branch.length *= branch_length_ratio;
			VerletPoint::ptr new_point = VerletPoint::create(last_point->coords.x, last_point->coords.y - branch.length, 10.0f);
			branch.rotate(new_point);
			const float angle = getRandRange(fork_amplitude);
			solver.addJoin(Join(last_base, last_point, new_point, angle, branch.length, 0.015f));
			solver.links.push_back(Link(last_point, new_point, 0.5f));

			generate(solver, branch, stage_count + 1U, max_stage);
		}

		// Continue the current branch
		Branch branch = last_branch;
		branch.length *= branch_length_ratio;
		VerletPoint::ptr new_point = VerletPoint::create(last_point->coords.x, last_point->coords.y - branch.length, 10.0f);
		branch.rotate(new_point);
		const float angle = getRandRange(branch_distortion);
		solver.addJoin(Join(last_base, last_point, new_point, angle, branch.length, 0.15f));
		solver.links.push_back(Link(last_point, new_point, 0.15f));
		
		generate(solver, branch, stage_count + 1U, max_stage);

	}

	Branch createBranch(Solver& solver, VerletPoint::ptr root)
	{
		float angle = -0.5f * PI;
		VerletPoint::ptr last_base = nullptr;
		VerletPoint::ptr last_point = root;
		VerletPoint::ptr new_point = VerletPoint::create(last_point->coords.x, last_point->coords.y - branch_length, 10.0f);

		solver.addJoin(Join(last_base, last_point, new_point, angle, branch_length, 0.05f));
		solver.links.push_back(Link(last_point, new_point));
		solver.points.push_back(new_point);

		Branch branch;
		branch.length = branch_length;
		branch.base = root;
		branch.extremity = new_point;

		return branch;
	}
};
