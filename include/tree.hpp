#pragma once

#include "join.hpp"


struct Branch
{
	float length;
	float angle_with_parent;
};


struct Tree
{
	float fork_angle;
	float fork_amplitude;
	
	float branch_length;
	float branch_length_ratio;
	float branch_width;
	float branch_width_ratio;


};
