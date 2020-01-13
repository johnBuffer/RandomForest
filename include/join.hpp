#pragma once

#include "verlet_point.hpp"
#include "utils.hpp"


struct Join
{
	Join()
		: point1(nullptr)
		, point2(nullptr)
		, point3(nullptr)
	{}

	Join(VerletPoint::ptr p1, VerletPoint::ptr p2, VerletPoint::ptr p3, float angle_, float length_, float strength_ = 1.0f)
		: point1(p1)
		, point2(p2)
		, point3(p3)
		, angle(angle_)
		, strength(strength_)
		, length(length_)
		, last_delta(0.0f)
	{
	}

	float getAngle1() const
	{
		return getAngle(point2, point1);
	}

	float getAngle2() const
	{
		return getAngle(point3, point2);
	}

	float getLength1() const
	{
		return getDistance(point1, point2);
	}

	float getLength2() const
	{
		return getDistance(point3, point2);
	}

	void update()
	{
		if (!point2 || !point3) {
			return;
		}

		const float angle1 = getAngle1();
		const float angle2 = getAngle2();
		const float current_angle = point1 ? getVec2Angle(point2->coords - point1->coords, point3->coords - point2->coords) : getVec2Angle(Vec2(1.0f, 0.0f), point3->coords - point2->coords);
		const float delta = angle - current_angle;

		const float delta_delta = last_delta - delta;
		float target_angle = 0.0f; 

		if (std::abs(delta_delta) < PI) {
			target_angle = angle2 + delta * strength;
			last_delta = delta;
		}
		else {
			target_angle = angle2 + last_delta * strength;
		}


		const float dx = cos(target_angle);
		const float dy = sin(target_angle);
		const Vec2 v = Vec2(dx, dy) * length;

		point3->moveTo(point2->coords + v);
		
	}

	float angle;
	float strength;
	float length;

	float last_delta;

	VerletPoint::ptr point1;
	VerletPoint::ptr point2;
	VerletPoint::ptr point3;

private:
	float getAngle(VerletPoint::ptr p1, VerletPoint::ptr p2) const
	{
		if (!p1 || !p2) {
			return 0.0f;
		}

		const Vec2 v1 = *p1 - *p2;
		return getVec2Angle(Vec2(1.0f, 0.0f), v1);
	}

	float getDistance(VerletPoint::ptr p1, VerletPoint::ptr p2) const
	{
		if (!p1 || !p2) {
			return 0.0f;
		}

		return getLength(*p1 - *p2);
	}
};
