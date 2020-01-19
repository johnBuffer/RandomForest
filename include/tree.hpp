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

		Branch branch = createTrunk(solver, root);

		generate(solver, branch, 0U, stage_count);
	}

	void generate(Solver& solver, Branch last_branch, uint32_t stage_count, uint32_t max_stage)
	{
		if (stage_count >= max_stage) {
			return;
		}

		const float fork_rand = getRandUnder(1.0f);
		// Here we fork
		if (fork_rand < fork_probability) {
			Branch new_branch;
			if (createBranch(solver, last_branch, new_branch, true, stage_count, max_stage)) {
				generate(solver, new_branch, stage_count + 1U, max_stage);
			}
		}

		// Continue the current branch
		Branch new_branch;
		if (createBranch(solver, last_branch, new_branch, false, stage_count, max_stage)) {
			generate(solver, new_branch, stage_count + 1U, max_stage);
		}

	}

	bool createBranch(Solver& solver, const Branch& last_branch, Branch& new_branch, bool is_fork, uint32_t stage_count, uint32_t max_stage)
	{
		Branch branch = last_branch;
		branch.length *= branch_length_ratio;
		
		if (branch.length < 2.0f) {
			return false;
		}

		VerletPoint::ptr last_base = last_branch.base;
		VerletPoint::ptr last_point = last_branch.extremity;

		const float join_strength = 0.05f;
		const float link_strenght = 0.99f;
		const float mass_factor = 0.25f;
		const float point_mass = mass_factor * float(max_stage) / float(stage_count * stage_count + 1.0f);
		const float angle = is_fork ? getRandRange(fork_amplitude) : getRandRange(branch_distortion);

		VerletPoint::ptr new_point = VerletPoint::create(last_point->coords.x, last_point->coords.y - branch.length, point_mass);
		branch.rotate(new_point);
		solver.addJoin(Join(last_base, last_point, new_point, angle, branch.length, join_strength));
		solver.points.push_back(new_point);

		new_branch = branch;

		return true;
	}

	Branch createTrunk(Solver& solver, VerletPoint::ptr& root)
	{
		float angle = -0.5f * PI;
		VerletPoint::ptr last_base = nullptr;
		VerletPoint::ptr last_point = root;
		VerletPoint::ptr new_point = VerletPoint::create(last_point->coords.x, last_point->coords.y - branch_length, 10.0f);

		solver.addJoin(Join(last_base, last_point, new_point, angle, branch_length, 0.25f));
		solver.points.push_back(new_point);

		Branch branch;
		branch.length = branch_length;
		branch.base = root;
		branch.extremity = new_point;

		return branch;
	}
};
