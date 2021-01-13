#pragma once
#include "physics.hpp"	
#include <iostream>


struct Solver
{
	std::vector<Node> nodes;
	std::vector<Segment> segments;
	std::vector<Joint> joints;

	Index<Node> addNode(float x, float y, float mass = 1.0f)
	{
		const uint64_t id = nodes.size();
		nodes.emplace_back(x, y, mass);
		return Index<Node>(id, nodes);
	}

	Index<Segment> addSegment(Index<Node>& node_1, Index<Node>& node_2)
	{
		const uint64_t id = segments.size();
		segments.emplace_back(node_1, node_2, (node_1->position - node_2->position).getLength());
		return Index<Segment>(id, segments);
	}

	void update(Joint& joint, float dt)
	{
		Node& p11 = *(joint.segment_1->point_1);
		Node& p12 = *(joint.segment_1->point_2);
		Node& p21 = *(joint.segment_2->point_1);
		Node& p22 = *(joint.segment_2->point_2);
		
		const Vec2 v_1 = p11.position - p12.position;
		const Vec2 v_2 = p21.position - p22.position;
		const float angle = Vec2::getAngle(v_1, v_2);
		const float delta = joint.target - angle;

		p22.position.rotate(p21.position, delta * joint.strength * dt);
	}
};
