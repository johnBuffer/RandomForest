#pragma once
#include "tree.hpp"
#include "grass/grass.hpp"
#include "wind.hpp"


struct LayerRender
{
	std::vector<sf::VertexArray> branches;
	sf::VertexArray leaves;
	sf::VertexArray grass_va;

	LayerRender()
		: branches()
		, leaves(sf::Quads)
		, grass_va(sf::Quads)
	{

	}
};


struct Layer
{
	// Trees
	std::vector<Tree> trees;
	// Grass
	Solver solver;
	std::vector<Grass> grass;
	// Render
	LayerRender render_data;

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

	void generateGrassRender()
	{
		render_data.grass_va.clear();
		for (Grass& g : grass) {
			g.addToVa(render_data.grass_va);
		}
	}

	void generateRenderArrays()
	{
		va.clear();
		for (Grass& g : grass) {
			g.addToVa(va);
		}
	}
};

