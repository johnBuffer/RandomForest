#pragma once
#include "vec2.hpp"
#include "index_as_pointer.hpp"


struct Node
{
	Vec2  position;
	bool  is_static;
	float mass;

	Node()
		: position()
		, is_static(false)
		, mass(1.0f)
	{}

	Node(float x, float y, float m = 1.0f)
		: position(x, y)
		, is_static(false)
		, mass(m)
	{}
};

struct Segment
{
	Index<Node> point_1;
	Index<Node> point_2;

	float length;

	Segment(Index<Node> p_1, Index<Node> p_2, float length_)
		: point_1(p_1)
		, point_2(p_2)
		, length(length_)
	{}
};

struct Joint
{
	Index<Segment> segment_1;
	Index<Segment> segment_2;

	float target;
	float strength;
};
