#pragma once
#include <vector>
#include <vec2.hpp>

struct Node
{
	Vec2 pos;
	float angle;
	float length;

	Node()
		: pos()
		, angle(0.0f)
		, length(0.0f)
	{}

	Node(float x, float y, float a, float l)
		: pos(x, y)
		, angle(a)
		, length(l)
	{}
};


struct TreeConf
{
	float branch_deviation;
	float branch_split_angle;
	float branch_split_var;
	float branch_length;
	float branch_length_ratio;
	float branch_split_proba;
};


struct GrowthResult
{
	bool split;
	Node node;
};


struct Branch
{
	Node current_node;

	Branch()
		: current_node()
	{}

	Branch(const Vec2& pos, float a, float l)
		: current_node(pos.x, pos.y, a, l)
	{}

	GrowthResult grow(const TreeConf& conf)
	{
		const float new_length = 
	}
};


struct Fork
{
	float max_angle;
	float length_ratio;
	int32_t forks_count;
};


struct Tree
{
	Branch root;

	Tree(const Vec2& pos)
	{
		float base_angle = 0.0f;
		float base_length = 100.0f;
		root = Branch(pos, base_angle, base_length);
	}

	void generateLevel()
	{

	}

	void generateSubs(Branch& b)
	{

	}
};
