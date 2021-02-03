#pragma once
#include <vector>


struct Vec2
{
	float x, y;

	Vec2()
		: x(0.0f)
		, y(0.0f)
	{}
};


struct Branch
{
	Vec2 position;
	float angle;
	float length;

	std::vector<Branch> subs;

	Branch()
		: position()
		, angle(0.0f)
		, length(0.0f)
	{}

	Branch(const Vec2& pos, float a, float l)
		: angle(a)
		, length(l)
	{}
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
