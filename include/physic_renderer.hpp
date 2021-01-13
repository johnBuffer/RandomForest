#pragma once
#include "solver.hpp"
#include <SFML/Graphics.hpp>

struct PhysicRenderer
{
	Solver& solver;

	PhysicRenderer(Solver& solver_)
		: solver(solver_)
	{}

	void render(sf::RenderTarget& target)
	{
		renderSegments(target);
		renderNodes(target);
	}

	void renderNodes(sf::RenderTarget& target)
	{
		const float r = 4.0f;
		sf::CircleShape c(r);
		c.setFillColor(sf::Color::Green);
		c.setOrigin(r, r);
		for (const Node& n : solver.nodes) {
			c.setPosition(n.position.x, n.position.y);
			target.draw(c);
		}
	}

	void renderSegments(sf::RenderTarget& target)
	{
		sf::VertexArray va(sf::Lines, 2 * solver.segments.size());
		uint64_t i(0);
		for (const Segment& s : solver.segments) {
			va[2 * i + 0].position = sf::Vector2f(s.point_1->position.x, s.point_1->position.y);
			va[2 * i + 1].position = sf::Vector2f(s.point_2->position.x, s.point_2->position.y);

			va[2 * i + 0].color = sf::Color::White;
			va[2 * i + 1].color = sf::Color::White;
			++i;
		}

		target.draw(va);
	}
};
