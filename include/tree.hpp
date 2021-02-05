#pragma once
#include <vector>
#include <vec2.hpp>
#include "utils.hpp"
#include "number_generator.hpp"
#include "wind.hpp"


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


struct Node
{
	Vec2 pos;
	Vec2 growth_direction;
	float length;
	float width;
	uint32_t level;
	uint32_t index;

	Node()
		: pos()
		, growth_direction()
		, length(0.0f)
		, width(0.0f)
		, level(1)
		, index(0)
	{}

	Node(float x, float y, float a, float l, float w, uint32_t lvl, uint32_t i)
		: pos(x, y)
		, growth_direction(cos(a), sin(a))
		, length(l)
		, width(w)
		, level(lvl)
		, index(i)
	{}

	Node(const Vec2& position, const Vec2& direction, float l, float w, uint32_t lvl, uint32_t i)
		: pos(position)
		, growth_direction(direction)
		, length(l)
		, width(w)
		, level(lvl)
		, index(i)
	{}

	Vec2 getEnd() const
	{
		return pos + (growth_direction * length);
	}
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
	std::vector<Node> nodes;

	Branch()
	{}

	Branch(Node node)
		: nodes{ node }
	{}

	Branch(const Vec2& pos, float a, float l, float width)
		: nodes{ Node(pos.x, pos.y, a, l, width, 1, 0) }
	{}

	GrowthResult grow(const TreeConf& conf)
	{
		GrowthResult result;
		const Node& current_node = nodes.back();
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
			nodes.emplace_back(start, direction, new_length, new_width, level, length + 1);
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


struct Leaf
{
	Vec2 attach;
	Vec2 position;
	Vec2 old_position;
	Vec2 target_direction;
	Vec2 acceleration;
	float joint_strenght;

	Leaf(const Vec2& pos, const Vec2& dir)
		: attach(pos)
		, position(pos + dir)
		, old_position(position)
		, target_direction(dir)
		, joint_strenght(RNGf::getRange(1.0f, 2.0f))
	{}

	void solveAttach()
	{
		const float length = 1.0f;
		const Vec2 delta = position - attach;
		const float dist_delta = 1.0f - delta.getLength();
		position = position + delta.getNormalized() * dist_delta;
	}

	Vec2 getDir() const
	{
		return position - attach;
	}

	void update(float dt)
	{
		solveAttach();
		const Vec2 velocity = position - old_position;
		const Vec2 new_pos = position + (velocity + acceleration * dt);
		old_position = position;
		position = new_pos;
		acceleration = target_direction * joint_strenght;
	}
};


struct Tree
{
	TreeConf conf;

	std::vector<Branch> branches;
	std::vector<Leaf> leafs;

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
		addLeafs();
	}

	void update(float dt, std::vector<Wind>& wind)
	{
		for (Leaf& l : leafs) {
			for (Wind& w : wind) {
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
				const float angle = RNGf::getRange(PI);
				leafs.push_back(Leaf(b.nodes[nodes_count - 1 - i].pos, Vec2(cos(angle), sin(angle))));
			}
		}
	}
};
