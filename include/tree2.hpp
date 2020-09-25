#pragma once
#include <index_vector.hpp>
#include "vec2.hpp"


struct Tree2
{
	struct Node
	{
		// The node's position
		Vec2 position;
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
	};

	IndexVector<Node> nodes;
};
