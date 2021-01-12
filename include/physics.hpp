#pragma once
#include "vec2.hpp"
#include "index_as_pointer.hpp"


struct Node
{
	Vec2 position;
};

struct Segment
{
	Index<Node> point_1;
	Index<Node> point_2;
};

struct Join
{
	Index<Segment> segment_1;
	Index<Segment> segment_2;

	float target;
	float strength;
};
