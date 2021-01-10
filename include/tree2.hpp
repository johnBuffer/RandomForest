#pragma once
#include <index_vector.hpp>
#include "vec2.hpp"


struct Tree2
{
	struct Node
	{
		Node() = default;
		Node(const Vec2& p, uint64_t b)
			: position(p)
			, branch(b)
		{}
		// The node's position
		Vec2 position;
		// The parent branch
		uint64_t branch;
	};

	struct Fork
	{
		Fork()
			: root(false)
		{}
		// The IDs of the segment containing the fork
		uint64_t p1;
		uint64_t p2;
		// The fork's position along the segment
		float ratio;
		bool root;
	};

	struct ProjectionPoint
	{
		ProjectionPoint()
			: valid(false)
		{}

		Fork fork;
		bool valid;
	};

	struct Branch
	{
		Branch() = default;
		Branch(const Fork& fork_)
			: fork(fork_)
		{}
		// The branch's root
		Fork fork;
		// The branch's nodes
		std::vector<uint64_t> nodes;
		// The connected branches
		std::vector<uint64_t> sub_branches;
	};

	struct Config
	{
		float branch_length;
		float branch_length_ratio;
		float branch_width;
	};

	// Contains all the tree's nodes
	IndexVector<Node> nodes;
	// Contains all the tree's branches
	IndexVector<Branch> branches;
	// Config
	Config config;
	

	// METHODS

	template<typename Vec2Type>
	void addBranch(const Vec2Type& position)
	{
		uint64_t node = nodes.emplace_back(Vec2(position.x, position.y), 0);
		uint64_t branch = branches.emplace_back()
	}

	void fork(uint64_t p1, uint64_t p2, float ratio)
	{
		// Get needed refs
		Node& n1 = getNode(p1);
		Node& n2 = getNode(p2);
		Branch& branch = getBranch(n1.branch);
		// Create fork
		Fork fork{ p1, p2, ratio };
		// Create branch
		uint64_t new_branch = branches.emplace_back(fork);
		branch.sub_branches.push_back(new_branch);
		// Create node
		uint64_t new_node = nodes.emplace_back(Vec2(), new_branch);
	}

	Node& getNode(uint64_t id)
	{
		return nodes[id];
	}

	const Node& getNode(uint64_t id) const
	{
		return nodes[id];
	}

	Branch& getBranch(uint64_t id)
	{
		return branches[id]; 
	}

	template<typename Vec2Type>
	ProjectionPoint getClosestSegment(const Vec2Type& pos) const
	{
		ProjectionPoint result;

		float min_dist = -1.0f;

		for (Branch& branch : branches) {
			int64_t nodes_count = branch.nodes.size();
			auto& nodes = branch.nodes;
			for (int64_t i(0); i < nodes_count - 1; ++i) {
				const Segment<Vec2Type> s(nodes[i], nodes[i + 1]);
				const Segment<Vec2Typef>::Projection p = s.getProjectionOf(current_point);
				if (p.k >= 0.0f && p.k <= s.size) {
					const float dist = Utils::getLength(p.point - current_point);
					if (dist < min_dist || min_dist < 0.0f) {
						min_dist = dist;
						result.fork.p1 = s.p1;
						result.fork.p2 = s.p2;
						result.fork.ratio = p.k / s.size;
						valid = true;
					}
				}
			}
		}

		return result;
	}
};
