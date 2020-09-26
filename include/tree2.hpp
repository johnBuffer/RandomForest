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
		/*Fork(uint64_t p1_, uint64_t p2_, float ratio_)
			: p1(p1_)
			, p2(p2_)
			, ratio(ratio_)
		{}*/
		// The IDs of the segment containing the fork
		uint64_t p1;
		uint64_t p2;
		// The fork's position along the segment
		float ratio;
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

	// Contains all the tree's nodes
	IndexVector<Node> nodes;
	// Contains all the tree's branches
	IndexVector<Branch> branches;

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
		// Create node
		uint64_t new_node = nodes.emplace_back(Vec2(), new_branch);
	}

	Node& getNode(uint64_t id)
	{
		return nodes[id];
	}

	Branch& getBranch(uint64_t id)
	{
		return branches[id]; 
	}
};
