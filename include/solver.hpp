#pragma once

#include <SFML/Graphics.hpp>
#include "link.hpp"
#include "join.hpp"
#include <vector>


// Create a "TypeMap" 


class Solver
{
public:
	Solver() = default;

	VerletPoint::ptr createPoint(float x, float y, float mass, bool moving = true)
	{
		VerletPoint::ptr pt = VerletPoint::create(x, y, mass);
		pt->moving = moving;
		m_points.push_back(pt);
		return pt;
	}

	Link::ptr createLink(VerletPoint::ptr pt1, VerletPoint::ptr pt2)
	{
		Link::ptr link = Link::create(pt1, pt2);
		m_links.push_back(link);
		return link;
	}

	Join::ptr createJoin(Link::ptr link_1, Link::ptr link_2)
	{
		Join::ptr join = Join::create(link_1, link_2);
		m_joins.push_back(join);
		return join;
	}

	void applyGravity()
	{
		for (VerletPoint::ptr pt : m_points) {
			pt->acceleration.y  = 500.0f;
		}
	}

	void update(float dt = 0.016f)
	{
		applyGravity();

		const uint32_t iterations_count = 4u;
		for (uint8_t i(iterations_count); i--;) {
			for (VerletPoint::ptr pt : m_points) {
				pt->update(dt);
			}

			const uint64_t links_count = m_links.size();
			for (uint64_t i(0u); i < links_count; ++i) {
				m_links[i]->update();
			}
		}
	}

	void render(sf::RenderTarget& target)
	{
		const float join_radius = 4.0f;
		for (const Join::ptr join : m_joins) {
			sf::CircleShape c(join_radius);
			c.setOrigin(join_radius, join_radius);
			c.setFillColor(sf::Color::Red);
			const VerletPoint::ptr pt = join->pt_2;
			c.setPosition(pt->coords.x, pt->coords.y);
			target.draw(c);
		}

		const float point_radius = 2.0f;
		for (const VerletPoint::ptr pt : m_points) {
			sf::CircleShape c(point_radius);
			c.setOrigin(point_radius, point_radius);
			c.setFillColor(sf::Color::Yellow);
			c.setPosition(pt->coords.x, pt->coords.y);
			target.draw(c);
		}
	}

private:
	std::vector<VerletPoint::ptr> m_points;
	std::vector<Link::ptr> m_links;
	std::vector<Join::ptr> m_joins;
};

