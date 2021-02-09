#pragma once
#include "tree.hpp"
#include "layer.hpp"
#include "swarm.hpp"


struct TreeInstance
{
	Tree tree;
	uint32_t picks_count;
	uint32_t refs_count;
};


struct Forest
{
	// System
	LayerConf conf;
	uint32_t layers_count;
	swrm::Swarm swarm;
	int32_t current_last;
	// Data
	std::vector<TreeInstance> trees;
	std::vector<Layer> layers;
	std::vector<Wind> wind;

	Forest(const LayerConf& config, uint32_t layers_count_)
		: conf(config)
		, layers_count(layers_count_)
		, swarm(layers_count_)
		, current_last(0)
	{

	}

	void initialize()
	{
		layers.clear();
		for (uint32_t i(layers_count); i--;) {
			layers.emplace_back(conf, float(i) * conf.depth_spacing);
			layers.back().init();
		}
	}

	void upadte(float dt)
	{
		for (Wind& w : wind) {
			w.update(dt, conf.width);
		}

		swrm::WorkGroup group = swarm.execute([&](uint32_t id, uint32_t group_size) {
			const float dist_threshold = 0.1f;
			Layer& layer = layers[id];
			if (layer.dist > dist_threshold) {
				layer.update(dt, wind);
				layer.generateRenderArrays(dist_threshold);
			}
			else {
				layer.dist = group_size * conf.depth_spacing;
				layer.back_to_end = true;
				layer.init();
			}
		}, layers_count);
		group.waitExecutionDone();
	}
};
