#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>


struct Branch
{
	std::vector<sf::Vector2f> nodes;
};


float getLength(const sf::Vector2f& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f getNormalized(const sf::Vector2f& v)
{
	return v / getLength(v);
}

struct Segment
{
	Segment(const sf::Vector2f& p1_, const sf::Vector2f& p2_)
		: p1(p1_)
		, p2(p2_)
		, v(getNormalized(p2 - p1))
		, size(getLength(p2 - p1))
	{}

	sf::Vector2f p1;
	sf::Vector2f p2;
	sf::Vector2f v;
	float size;
};

struct Projection
{
	float k;
	sf::Vector2f point;
};

Projection getProjectionOf(const sf::Vector2f& point, const Segment& s)
{
	const float k = point.x * s.v.x + point.y * s.v.y - s.p1.x * s.v.x - s.v.y * s.p1.y;
	return Projection{ k, s.p1 + k * s.v };
}


int main()
{
	//srand(time(0));

    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	sf::Vector2i clic_pos;
	bool clicking = false;

	const float branch_length = 40.0f;
	const float branch_length_ratio = 0.95f;
	const float branch_width = 0.5f;

	float current_length;

	std::vector<Branch> branches;

	std::vector<sf::VertexArray> va;

	Segment test_segment(sf::Vector2f(150, 200), sf::Vector2f(1500, 800));

	while (window.isOpen())
	{
		auto mouse_pos = sf::Mouse::getPosition(window);
		const sf::Vector2f current_point = sf::Vector2f(mouse_pos.x, mouse_pos.y);

        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				branches.emplace_back();

				float min_dist = -1.0f;
				sf::Vector2f min_pos = current_point;
				float length = branch_length;

				for (Branch& branch : branches) {
					uint64_t nodes_count = branch.nodes.size();
					auto& nodes = branch.nodes;
					if (nodes_count > 1) {
						for (uint64_t i(0); i < nodes_count - 1; ++i) {
							const Segment s(nodes[i], nodes[i + 1]);
							const Projection p = getProjectionOf(current_point, s);
							if (p.k >= 0.0f && p.k <= s.size) {
								const float dist = getLength(p.point - current_point);
								if (dist < min_dist || min_dist < 0.0f) {
									min_dist = dist;
									min_pos = p.point;
									length = s.size * branch_length_ratio * 0.75f;
								}
							}
						}
					}
				}

				clicking = true;
				current_length = length;
				branches.back().nodes.emplace_back(min_pos.x, min_pos.y);
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				clicking = false;
			}
		}

		if (!branches.empty()) {
			if (clicking) {
				const sf::Vector2f last_point = branches.back().nodes.back();
				sf::Vector2f to_last_point = current_point - last_point;
				const float length = getLength(to_last_point);

				if (length > current_length) {
					branches.back().nodes.emplace_back(last_point + current_length * getNormalized(to_last_point));
					current_length *= branch_length_ratio;
				}
			}
			
			va.clear();
			for (Branch& branch : branches) {
				uint64_t nodes_count = branch.nodes.size();
				auto& nodes = branch.nodes;
				va.emplace_back(sf::TriangleStrip, 4 * (nodes_count - 1));
				if (nodes_count > 1) {
					for (uint64_t i(0); i < nodes_count - 1; ++i) {
						const sf::Vector2f vec = nodes[i + 1] - nodes[i];
						const float length = getLength(vec);
						const sf::Vector2f vec_normalized = vec / length;
						const sf::Vector2f vec_normal(-vec_normalized.y, vec_normalized.x);
						const float width = length * branch_length_ratio * 0.5f;

						va.back()[4 * i + 0].position = nodes[i] + width * vec_normal;
						va.back()[4 * i + 1].position = nodes[i] - width * vec_normal;
						va.back()[4 * i + 2].position = nodes[i + 1] + width * branch_length_ratio * vec_normal;
						va.back()[4 * i + 3].position = nodes[i + 1] - width * branch_length_ratio * vec_normal;
					}
				}
			}
		}

		float min_dist = -1.0f;
		sf::Vector2f min_pos;
		for (Branch& branch : branches) {
			uint64_t nodes_count = branch.nodes.size();
			auto& nodes = branch.nodes;
			if (nodes_count > 1) {
				for (uint64_t i(0); i < nodes_count - 1; ++i) {
					const Segment s(nodes[i], nodes[i+1]);
					const Projection p = getProjectionOf(current_point, s);
					if (p.k >= 0.0f && p.k <= s.size) {
						const float dist = getLength(p.point - current_point);
						if (dist < min_dist || min_dist < 0.0f) {
							min_dist = dist;
							min_pos = p.point;
						}
					}
				}
			}
		}

		window.clear(sf::Color::Black);

		for (const sf::VertexArray& va_ : va) {
			window.draw(va_);
		}
		
		const float radius = 4.0f;
		sf::CircleShape circle(radius);
		circle.setOrigin(radius, radius);
		circle.setPosition(min_pos);
		circle.setFillColor(sf::Color::Green);
		window.draw(circle);

        window.display();
    }

    return 0;
}
