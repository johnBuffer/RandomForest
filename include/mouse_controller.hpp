#pragma once
#include "solver.hpp"
#include <SFML/System.hpp>

struct MouseController
{
	Solver& solver;

	MouseController(Solver& solver_)
		: solver(solver_)
	{}

	Node* getSelected(const sf::Vector2i& mouse_position)
	{
		const Vec2 mouse_pos = vec2i_to_vec2f(mouse_position);
		const float radius = 8.0f;

		float min_dist = -1.0f;
		Node* result = nullptr;
		for (Node& n : solver.nodes) {
			const float dist = (n.position - mouse_pos).getLength();
			if (dist < radius) {
				if (dist < min_dist || min_dist == -1.0f) {
					min_dist = dist;
					result = &n;
				}
			}
		}

		return result;
	}

	static Vec2 vec2i_to_vec2f(const sf::Vector2i& v)
	{
		return Vec2(static_cast<float>(v.x), static_cast<float>(v.y));
	}

};
