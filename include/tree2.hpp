#pragma once
#include <index_vector.hpp>
#include "vec2.hpp"


struct Tree2
{
	struct Node
	{
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
		// The IDs of the segment containing the fork
		uint64_t p1, p2;
		// The ID of the fork's node
		uint64_t node;
	};

	struct Branch
	{
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

	void fork(uint64_t p1, uint64_t p2)
	{
		// Get needed refs
		Node& n1 = getNode(p1);
		Node& n2 = getNode(p2);
		Branch& branch = getBranch(n1.branch);
		// Add node
		uint64_t new_node = nodes.emp
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
