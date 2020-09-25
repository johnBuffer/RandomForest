#pragma once
#include "utils.hpp"


template<typename Vec2Type>
struct Segment
{
	struct Projection
	{
		Vec2Type point;
		float k;
	};

	Segment(const Vec2Type& p1_, const Vec2Type& p2_)
		: p1(p1_)
		, p2(p2_)
		, v(Utils::getNormalized(p2 - p1))
		, size(Utils::getLength(p2 - p1))
	{}

	Projection getProjectionOf(const Vec2Type& point) const
	{
		const float k = point.x * v.x + point.y * v.y - p1.x * v.x - v.y * p1.y;
		return Projection{ p1 + k * v, k };
	}

	static Projection getProjectionOf(const Vec2Type& point, const Segment<Vec2Type>& s)
	{
		const float k = point.x * s.v.x + point.y * s.v.y - s.p1.x * s.v.x - s.v.y * s.p1.y;
		return Projection{ s.p1 + k * s.v, k };
	}


	Vec2Type p1, p2, v;
	float size;
};