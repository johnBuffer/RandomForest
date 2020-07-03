#pragma once

#include "link.hpp"


struct Join
{
	using ptr = std::shared_ptr<Join>;

	Join(Link::ptr link_1_, Link::ptr link_2_, float target_angle_, float strength_)
		: target_angle(target_angle_)
		, strength(strength_)
	{
		if (&(*link_1_->point1) == &(*link_2_->point1)) {
			setPoints(link_1_->point2, link_1_->point1, link_2_->point2);
		}
		else if (&(*link_1_->point1) == &(*link_2_->point2)) {
			setPoints(link_1_->point2, link_1_->point1, link_2_->point1);
		}
		else if (&(*link_1_->point2) == &(*link_2_->point1)) {
			setPoints(link_1_->point1, link_1_->point2, link_2_->point2);
		}
		else {
			setPoints(link_1_->point1, link_1_->point2, link_2_->point1);
		}
	}

	void setPoints(VerletPoint::ptr pt_1_, VerletPoint::ptr pt_2_, VerletPoint::ptr pt_3_)
	{
		pt_1 = pt_1_;
		pt_2 = pt_2_;
		pt_3 = pt_3_;

		link_length_1 = (pt_1->coords - pt_2->coords).getLength();
		link_length_2 = (pt_3->coords - pt_2->coords).getLength();
	}

	void update(float dt)
	{
		const Vec2 v1 = pt_2->coords - pt_1->coords;
		const Vec2 v2 = pt_3->coords - pt_2->coords;

		const float length = v2.getLength();
		const Vec2 normalized_v2 = v2.getNormalized() * link_length_2;

		const float current_angle = getVec2Angle(v1, v2);
		const float delta_angle = 0.5f * strength * (target_angle - current_angle);

		const float cos_a_1 = cos(delta_angle);
		const float sin_a_1 = sin(delta_angle);
		const float d_x = normalized_v2.x * cos_a_1 - normalized_v2.y * sin_a_1;
		const float d_y = normalized_v2.x * sin_a_1 + normalized_v2.y * cos_a_1;

		//std::cout << d_x << " " << d_y << std::endl;

		pt_3->moveTo(pt_2->coords + Vec2(d_x, d_y));

		/*const Vec2 vec_2(-v1.x, -v1.y);
		const float d_x_2 = vec_2.x * cos_a_1 + vec_2.y * sin_a_1;
		const float d_y_2 = -vec_2.x * sin_a_1 + vec_2.y * cos_a_1;*/

		//pt_1->moveTo(pt_2->coords + Vec2(d_x_2, d_y_2));
	}

	template<typename... Args>
	static ptr create(Args&&... args)
	{
		return std::make_shared<Join>(args...);
	}

	float strength;
	float target_angle;

	float link_length_1;
	float link_length_2;

	VerletPoint::ptr pt_1;
	VerletPoint::ptr pt_2;
	VerletPoint::ptr pt_3;
};

