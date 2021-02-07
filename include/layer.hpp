#pragma once
#include "tree.hpp"
#include "grass/grass.hpp"
#include "wind.hpp"


struct Layer
{
	// Trees
	std::vector<Tree> trees;
	// Grass
	Solver solver;
	std::vector<Grass> grass;

	void init()
	{
		trees.clear();
		grass.clear();
		solver.clear();
	}

	void update(float dt, const std::vector<Wind>& wind)
	{
		// Wind on grass
		for (const Wind& w : wind) {
			for (VerletPoint::ptr pt : solver.getPoints()) {
				if (w.isOver(pt->coords)) {
					pt->acceleration += Vec2(w.strength, 0.0f);
				}
			}
		}
		// Update entities
		updateGrass(dt);
		updateTrees(dt, wind);
	}

	void updateTrees(float dt, const std::vector<Wind>& wind)
	{
		for (Tree& t : trees) {
			t.update(dt, wind);
		}
	}

	void updateGrass(float dt)
	{
		solver.update(dt);
	}
};

