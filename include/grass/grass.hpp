#pragma once

#include "number_generator.hpp"
#include "solver.hpp"

struct Grass
{
	Grass()
	{
		color = sf::Color::White;
	}

	static Grass add(Solver& solver, float x, float y)
	{
		Grass grass;

		float strength = 0.04f;
		float base_length = 10.0f;
		float link_length = base_length + rand() % (2 * int(base_length));
		const uint32_t points_count = 2;
		VerletPoint::ptr last_point = solver.createPoint(x, y, 1.0f, false);
		grass.points.push_back(last_point);
		Link::ptr last_link = nullptr;
		for (uint32_t i(0); i < points_count; ++i) {
			VerletPoint::ptr new_point = solver.createPoint(last_point->coords.x + RNGf::getRange(5.0f), last_point->coords.y - link_length, 0.01f / float(i + 1), i > 0);
			grass.points.push_back(new_point);
			Link::ptr new_link = solver.createLink(last_point, new_point);
			last_point = new_point;
			if (i > 0) {
				solver.createJoin(last_link, new_link, getRandRange(0.2f), strength);
				strength *= 2.0f;
			}
			link_length *= 0.8f;
			last_link = new_link;
		}

		return grass;
	}

	void addToVa(sf::VertexArray& va)
	{
		const float initial_width = 1.0f;
		float width = initial_width;
		const uint64_t points_count = points.size();
		for (uint32_t i(0); i < points_count - 1; ++i) {
			const Vec2& current_pt = points[i]->coords;
			const Vec2& next_pt    = points[i + 1]->coords;
			const Vec2 v = (next_pt - current_pt).getNormalized();

			sf::Vertex v1(sf::Vector2f(current_pt.x - width * v.y, current_pt.y + width * v.x), color);
			va.append(v1);
			sf::Vertex v2(sf::Vector2f(current_pt.x + width * v.y, current_pt.y - width * v.x), color);
			va.append(v2);

			width *= 0.5f;
			sf::Vertex v3(sf::Vector2f(next_pt.x + width*v.y, next_pt.y - width * v.x), color);
			va.append(v3);
			sf::Vertex v4(sf::Vector2f(next_pt.x - width * v.y, next_pt.y + width * v.x), color);
			va.append(v4);
		}
	}

	sf::Color color;
	std::vector<VerletPoint::ptr> points;
};
