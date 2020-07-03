#pragma once

#include "link.hpp"


struct Join
{
	using ptr = std::shared_ptr<Join>;

	Join(Link::ptr link_1_, Link::ptr link_2_) {
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
	}

	static ptr create(Link::ptr link_1_, Link::ptr link_2_)
	{
		return std::make_shared<Join>(link_1_, link_2_);
	}

	VerletPoint::ptr pt_1;
	VerletPoint::ptr pt_2;
	VerletPoint::ptr pt_3;
};

