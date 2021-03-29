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
	Node::Ptr root;

	GrowthResult()
		: split(false)
		, node()
		, root(nullptr)
	{}
};


struct Branch
{
	Node::Ptr root;
	Vec2 last_root_pos;
	std::vector<Node::Ptr> nodes;

	Branch() = default;

	Branch(Node::Ptr root_node, const Node& node)
		: root(root_node)
		, last_root_pos(root->pos)
		, nodes{ Node::create(node) }
	{}

	Branch(Node::Ptr node, const Vec2& pos, float a, float l, float width)
		: root(node)
		, last_root_pos(root->pos)
		, nodes{ Node::create(pos.x, pos.y, a, l, width, 1, 0) }
	{}

	GrowthResult grow(const TreeConf& conf)
	{
		GrowthResult result;
		Node& current_node = *(nodes.back());
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
			nodes.push_back(new_node);
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

	void update()
	{
		const Vec2 delta_pos = root->pos - last_root_pos;
		for (Node::Ptr n : nodes) {
			n->pos += delta_pos;
		}
		last_root_pos = root->pos;
	}
};


struct Tree
{
	TreeConf conf;

	Node::Ptr root;
	std::vector<Branch> branches;
	std::vector<Leaf> leaves;
	std::vector<PinnedSegment> segments;


	Tree(const Vec2& pos, const TreeConf& tree_conf)
		: conf(tree_conf)
	{
		float base_angle = -PI * 0.5f;
		root = Node::create(pos);
		branches.emplace_back(root, pos, base_angle, conf.branch_length, conf.branch_width);
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
		std::vector<GrowthResult> to_add;
		for (Branch& b : branches) {
			GrowthResult res = b.grow(conf);
			if (res.split) {
				to_add.emplace_back(res);
			}
		}

		for (GrowthResult& res : to_add) {
			branches.emplace_back(res.root, res.node);
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
		addLeaves();
	}

	void createSkeleton()
	{
		uint64_t i(0);
		for (Branch& b : branches) {
			Vec2 free_point = b.nodes.back()->pos;
			const float strength = 300.0f * static_cast<float>(std::pow(0.7f, b.root->level));
			segments.emplace_back(b.nodes.front(), free_point, i, strength);
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

		// Follow root
		for (Branch& b : branches) {
			b.update();
		}

		for (Leaf& l : leaves) {
			for (const Wind& w : wind) {
				if (w.isOver(l.free_particule.position)) {
					l.applyWind(w);
				}
			}
			
			l.update(dt);
		}
	}

	void applyWind(const std::vector<Wind>& wind)
	{
		for (const Wind& w : wind) {
			for (PinnedSegment& p : segments) {
				if (w.isOver(p.particule.position)) {
					const float strength_ratio = 1.0f - std::abs(w.pos_x - p.particule.position.x) / (0.5f * w.width);
					p.particule.acceleration += Vec2(1.0f, RNGf::getRange(0.5f)) * (strength_ratio * w.strength);
				}
			}
		}
	}

	void addLeaves()
	{
		for (Branch& b : branches) {
			const uint64_t nodes_count = b.nodes.size();
			const uint64_t leafs_count = 10;
			for (uint64_t i(0); i < std::min(leafs_count, nodes_count); ++i) {
				const float angle = RNGf::getRange(2.0f * PI);
				leaves.push_back(Leaf(b.nodes[nodes_count - 1 - i], Vec2(cos(angle), sin(angle))));
				leaves.back().size = 1.0f + (1.0f * i / float(leafs_count));
			}
		}
	}
};
