#pragma once
#include "tree.hpp"
#include "grass/grass.hpp"
#include "wind.hpp"
#include "layer_render.hpp"
#include "tree_builder.hpp"


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
		const uint32_t trees_count = RNGf::getUnder(config.trees_count);
		for (uint32_t i(trees_count); i--;) {
			Vec2 tree_pos(RNGf::getUnder(trees_zone), config.height);
			if (tree_pos.x > trees_zone * 0.5f) {
				tree_pos.x += config.gap;
			}

			TreeConf conf = config.tree_conf;
			const float size_var = RNGf::getRange(-20.0f, 20.0f);
			conf.branch_length += size_var;
			conf.branch_width += 2.0f * size_var;
			conf.branch_deviation += -0.1f * std::min(size_var, 0.0f);

			trees.push_back(Tree(tree_pos));
			trees.back();
		}

		for (float x(0.0f); x < config.width; x += 1.0f) {
			grass.push_back(Grass::add(solver, x, config.height, RNGf::getRange(4.0f, 18.0f)));
		}

		render_data.init(trees, grass);
	}

	void update(float dt, const std::vector<Wind>& wind)
	{
		// Wind on grass
		for (const Wind& w : wind) {
			for (VerletPoint::ptr pt : solver.getPoints()) {
				if (w.isOver(pt->coords)) {
					pt->acceleration += Vec2(w.strength * 5.0f, 0.0f);
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

	void generateRenderArrays(float dist_threshold)
	{
		float opacity = 1.0f;
		const float threshold_factor = 5.0f;
		if (dist < threshold_factor * dist_threshold) {
			opacity = (dist - dist_threshold) / ((threshold_factor - 1.0f) * dist_threshold);
		}
		
		render_data.render(trees, opacity);
		render_data.render(grass, opacity);
	}
};

