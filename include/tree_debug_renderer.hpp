#pragma once
#include <SFML/Graphics.hpp>
#include "tree.hpp"


struct TreeDebugRenderer
{
private:
	sf::RenderTarget& m_target;

public:
	TreeDebugRenderer(sf::RenderTarget& target)
		: m_target(target)
	{}

	void render(const Tree& tree)
	{
		const uint64_t lines_count = tree.segments.size();
		sf::VertexArray va(sf::Lines, 2 * lines_count);
		
		uint64_t i(0);
		for (const PinnedSegment& s : tree.segments) {
			Vec2 attach = s.attach->pos;
			va[2 * i].position = sf::Vector2f(attach.x, attach.y);
			va[2 * i + 1].position = sf::Vector2f(s.particule.position.x, s.particule.position.y);
			va[2 * i].color = sf::Color::Red;
			va[2 * i + 1].color = sf::Color::Red;
			++i;
		}

		m_target.draw(va);
	}
};
