#pragma once
#include <vector>
#include <vec2.hpp>
#include "utils.hpp"
#include "number_generator.hpp"
#include "wind.hpp"
#include "leaf.hpp"
#include "node.hpp"
#include "pinned_segment.hpp"


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

	GrowthResult()
		: split(false)
		, node()
	{}
};


struct Branch
{
	std::vector<Node::Ptr> nodes;

	Branch()
	{}

	Branch(Node node)
		: nodes{ Node::create(node) }
	{}

	Branch(const Vec2& pos, float a, float l, float width)
		: nodes{ Node::create(pos.x, pos.y, a, l, width, 1, 0) }
	{}

	GrowthResult grow(const TreeConf& conf)
	{
		GrowthResult result;
		Node& current_node = *(nodes.back());
		const uint32_t level = current_node.level;
		const uint32_t length = current_node.index;

		const float width = current_node.width;
		if (width > 0.3f) {
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
			nodes.emplace_back(Node::create(start, direction, new_length, new_width, level, length + 1));
			// Check for split
			if (length && (length % 3 == 0) && level < conf.max_level) {
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
			}
		}

		return result;
	}
};


struct Tree
{
	TreeConf conf;

	std::vector<Branch> branches;
	std::vector<Leaf> leafs;
	std::vector<PinnedSegment> segments;


	Tree(const Vec2& pos, const TreeConf& tree_conf)
		: conf(tree_conf)
	{
		float base_angle = -PI * 0.5f;
		branches.emplace_back(pos, base_angle, conf.branch_length, conf.branch_width);
	}

	uint64_t getNodesCount() const
	{
		uint64_t res = 0;
		for (const Branch& b : branches) {
			res += b.nodes.size();
		}
		return res;
	}

	void grow()
	{
		std::vector<Branch> to_add;
		for (Branch& b : branches) {
			GrowthResult res = b.grow(conf);
			if (res.split) {
				to_add.emplace_back(res.node);
			}
		}

		for (Branch& b : to_add) {
			branches.push_back(b);
		}
	}

	void fullGrow()
	{
		uint64_t nodes_count = 0;
		while (true) {
			grow();
			if (nodes_count == getNodesCount()) {
				break;
			}
			nodes_count = getNodesCount();
		}
		createSkeleton();
		addLeafs();
	}

	void createSkeleton()
	{
		uint64_t i(0);
		for (Branch& b : branches) {
			Vec2 free_point = b.nodes.back()->pos;
			segments.emplace_back(b.nodes.front(), free_point, i);
			++i;
		}
	}

	void rotateBranch(Branch& branch, Vec2 attach, float angle)
	{
		for (Node::Ptr n : branch.nodes) {
			n->pos.rotate(attach, angle);
		}
	}

	void update(float dt, const std::vector<Wind>& wind)
	{
		for (PinnedSegment& p : segments) {
			p.update(dt);
		}

		for (PinnedSegment& p : segments) {
			rotateBranch(branches[p.branch_id], p.attach->pos, p.delta_angle);
		}

		for (Leaf& l : leafs) {
			for (const Wind& w : wind) {
				if (w.isOver(l.position)) {
					l.acceleration += Vec2(1.0f, RNGf::getRange(1.0f)) * w.strength;
				}
			}
			
			l.update(dt);
		}
	}

	void addLeafs()
	{
		for (Branch& b : branches) {
			/*for (Node& n : b.nodes) {

			}*/
			const uint64_t nodes_count = b.nodes.size();
			const uint64_t leafs_count = 10;
			for (uint64_t i(0); i < std::min(leafs_count, nodes_count); ++i) {
				const float angle = RNGf::getRange(2.0f * PI);
				leafs.push_back(Leaf(b.nodes[nodes_count - 1 - i]->pos, Vec2(cos(angle), sin(angle))));
			}
		}
	}
};
