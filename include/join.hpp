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

		const float current_angle = getCurrentAngle();
		const float delta = angle - current_angle;

		/*std::cout << angle << std::endl;
		std::cout << current_angle << std::endl;
		std::cout << delta << std::endl;
		std::cout << std::endl;*/

		const Vec2& current_position = point3->coords;
		const Vec2 updated_position = rotate(current_position, delta * strength, point2->coords);

		point3->moveTo(updated_position);
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

	float getCurrentAngle() const
	{
		const Vec2 v2 = point3->coords - point2->coords;
		if (!point1) {
			return getVec2Angle(Vec2(1.0f, 0.0f), v2);
		}

		const Vec2 v1 = point2->coords - point1->coords;
		return getVec2Angle(v1, v2);
	}

	float getDistance(VerletPoint::ptr p1, VerletPoint::ptr p2) const
	{
		if (!p1 || !p2) {
			return 0.0f;
		}

		return getLength(*p1 - *p2);
	}
};
