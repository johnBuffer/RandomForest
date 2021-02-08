#pragma once
#include "tree.hpp"
#include "grass/grass.hpp"
#include "wind.hpp"
#include "layer_render.hpp"


struct LayerConf
{
	uint32_t trees_count;
	float width;
	float height;
	float gap;
	TreeConf tree_conf;
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
	// Conf
	LayerConf config;
	// Distance to camera
	float dist;
	bool back_to_end;

	Layer(const LayerConf& conf, float dist_)
		: render_data(conf.width, conf.height)
		, config(conf)
		, dist(dist_)
		, back_to_end(false)
	{}

	void init()
	{
		trees.clear();
		grass.clear();
		solver.clear();

		const float trees_zone = config.width - config.gap;
		for (uint32_t i(config.trees_count); i--;) {
			Vec2 tree_pos(RNGf::getUnder(trees_zone), config.height);
			if (tree_pos.x > trees_zone * 0.5f) {
				tree_pos.x += config.gap;
			}
			trees.push_back(Tree(tree_pos, config.tree_conf));
			trees.back().fullGrow();
		}

		for (float x(0.0f); x < config.width; x += 1.0f) {
			grass.push_back(Grass::add(solver, x, config.height, RNGf::getRange(4.0f, 12.0f)));
		}

		render_data.init(trees, grass);
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
			t.applyWind(wind);
			t.update(dt, wind);
		}
	}

	void updateGrass(float dt)
	{
		solver.update(dt);
	}

	void generateGrassRenderData()
	{
	}

	void generateTreesRenderData()
	{
		
	}

	void generateRenderArrays()
	{
		render_data.render(trees);
		render_data.render(grass);
	}
};

