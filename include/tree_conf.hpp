#pragma once
#include "vec2.hpp"
#include <cstdint>


struct TreeConf
{
	float branch_width;
	float branch_width_ratio;
	float split_width_ratio;
	float branch_deviation;
	float branch_split_angle;
	float branch_split_var;
	float branch_length;
	float branch_length_ratio;
	float branch_split_proba;
	float double_split_proba;
	Vec2 attraction;
	uint32_t max_level;
};