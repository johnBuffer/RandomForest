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

	Index<Joint> addJoint(Index<Segment>& segment_1, Index<Segment>& segment_2, float target, float strength)
	{
		const uint64_t id = joints.size();
		joints.emplace_back(segment_1, segment_2, target, strength);
		return Index<Joint>(id, joints);
	}

	void update(float dt)
	{
		applyGravity();
		for (Joint& j : joints) {
			update(j, dt);
		}

		for (Segment& s : segments) {
			update(s, dt);
		}

		for (Node& n : nodes) {
			n.integrate(dt);
		}
	}

	void update(Segment& segment, float dt)
	{
		Node& p_1 = *(segment.point_1);
		Node& p_2 = *(segment.point_2);

		const Vec2 v = p_2.position - p_1.position;
		const Vec2 v_nrm = v.getNormalized();
		const float delta = 0.5f * (segment.length - v.getLength());

		if (!p_1.is_static) {
			p_1.position -= v_nrm * delta;
			p_1.velocity -= v_nrm * delta;
		}

		if (!p_2.is_static) {
			p_2.position += v_nrm * delta;
			p_2.velocity += v_nrm * delta;
		}
	}

	void update(Joint& joint, float dt)
	{
		Node& p_11 = *(joint.segment_1->point_1);
		Node& p_12 = *(joint.segment_1->point_2);
		Node& p_21 = *(joint.segment_2->point_1);
		Node& p_22 = *(joint.segment_2->point_2);
		
		const Vec2 v_1 = p_11.position - p_12.position;
		const Vec2 v_2 = p_21.position - p_22.position;
		const float angle = Vec2::getAngle(v_1, v_2);
		const float delta = joint.target - angle;

		const Vec2 old_position = p_22.position;
		p_22.position.rotate(p_21.position, delta * joint.strength * dt);
		p_22.velocity += p_22.position - old_position;
	}

	void applyGravity()
	{
		const Vec2 gravity(0.0f, 1000.0f);
		for (Node& n : nodes) {
			n.accelerate(gravity);
		}
	}
};
