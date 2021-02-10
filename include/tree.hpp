#pragma once
#include <vector>
#include <vec2.hpp>
#include "utils.hpp"
#include "number_generator.hpp"
#include "wind.hpp"
#include "leaf.hpp"
#include "node.hpp"
#include "pinned_segment.hpp"


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
	Node::Ptr root;
	std::vector<Branch> branches;
	std::vector<Leaf> leaves;
	std::vector<PinnedSegment> segments;


	Tree(const Vec2& pos)
	{
		root = Node::create(pos);
	}

	uint64_t getNodesCount() const
	{
		uint64_t res = 0;
		for (const Branch& b : branches) {
			res += b.nodes.size();
		}
		return res;
	}

	void createSkeleton()
	{
		uint64_t i(0);
		for (Branch& b : branches) {
			Vec2 free_point = b.nodes.back()->pos;
			const float strength = 500.0f * static_cast<float>(std::pow(0.8f, b.root->level));
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

		/*for (Leaf& l : leaves) {
			for (const Wind& w : wind) {
				if (w.isOver(l.free_particule.position)) {
					l.applyWind(w);
				}
			}
			
			l.update(dt);
		}*/
	}

	void applyWind(const std::vector<Wind>& wind)
	{
		for (const Wind& w : wind) {
			for (PinnedSegment& p : segments) {
				if (w.isOver(p.particule.position)) {
					p.particule.acceleration += Vec2(1.0f, RNGf::getRange(2.0f)) * w.strength;
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
				leaves.back().size = 1.0f + (2.0f * i / float(leafs_count));
			}
		}
	}
};
