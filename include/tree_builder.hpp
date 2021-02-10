#pragma once
#include <cstdint>
#include "vec2.hpp"
#include "tree.hpp"


struct TreeConf
{
	float branch_width;
	float branch_width_ratio;
	float split_width_ratio;
	float branch_deviation;
	float branch_split_angle;
	float branch_split_var;
	float branch_length;
	float branch_length_ratio;
	float branch_split_proba;
	float double_split_proba;
	Vec2 attraction;
	uint32_t max_level;
};


struct GrowthResult
{
	bool split;
	Node node;
	Node::Ptr root;

	GrowthResult()
		: split(false)
		, node()
		, root(nullptr)
	{}
};


struct TreeBuilder
{
	static GrowthResult grow(Branch& branch, const TreeConf& conf)
	{
		GrowthResult result;
		Node& current_node = *branch.nodes.back();
		const uint32_t level = current_node.level;
		const uint32_t index = current_node.index;

		const float width = current_node.width;
		const float width_threshold = 0.8f;
		if (width > width_threshold) {
			// Compute new start
			const Vec2 start = current_node.getEnd();
			// Compute new length
			const float new_length = current_node.length * conf.branch_length_ratio;
			const float new_width = current_node.width * conf.branch_width_ratio;
			// Compute new direction
			const float deviation = getRandRange(conf.branch_deviation);
			Vec2 direction = current_node.growth_direction;
			direction.rotate(deviation);
			const float attraction_force = 1.0f / new_length;
			direction = (direction + conf.attraction * attraction_force).getNormalized();
			// Add new node
			Node::Ptr new_node = Node::create(start, direction, new_length, new_width, level, index + 1);
			branch.nodes.push_back(new_node);
			// Check for split
			if (index && (index % 5 == 0) && level < conf.max_level) {
				result.root = new_node;
				result.split = true;
				float split_angle = conf.branch_split_angle + getRandRange(conf.branch_split_var);
				// Determine side
				if (RNGf::rng(0.5f)) {
					split_angle = -split_angle;
				}
				result.node.pos = start;
				result.node.growth_direction = Vec2::getRotated(direction, split_angle);
				result.node.length = new_length * conf.branch_length_ratio;
				result.node.width = new_width * conf.split_width_ratio;
				result.node.level = level + 1;
				result.node.index = 0;
				// Avoid single node branches
				if (result.node.width < width_threshold) {
					result.split = false;
					new_node->width = 0.0f;
				}
			}
		}

		return result;
	}

	void grow(Tree& tree, const TreeConf& conf)
	{
		std::vector<GrowthResult> to_add;
		for (Branch& b : tree.branches) {
			GrowthResult res = TreeBuilder::grow(b, conf);
			if (res.split) {
				to_add.emplace_back(res);
			}
		}

		for (GrowthResult& res : to_add) {
			tree.branches.emplace_back(res.root, res.node);
		}
	}

	void fullGrow(Tree& tree, const TreeConf& conf)
	{
		// Create root
		float base_angle = -PI * 0.5f;
		tree.branches.emplace_back(tree.root, tree.root->pos, base_angle, conf.branch_length, conf.branch_width);
		// Build the tree
		uint64_t nodes_count = 0;
		while (true) {
			grow(tree, conf);
			if (nodes_count == tree.getNodesCount()) {
				break;
			}
			nodes_count = tree.getNodesCount();
		}
		// Add physic and leaves
		tree.createSkeleton();
		tree.addLeaves();
	}
};
