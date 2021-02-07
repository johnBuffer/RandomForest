#pragma once
#include <cstdint>
#include "vec2.hpp"


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
