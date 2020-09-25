#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>
#include "segment.hpp"
#include "utils.hpp"


struct Branch
{
	std::vector<sf::Vector2f> nodes;
};



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

	Segment<sf::Vector2f> test_segment(sf::Vector2f(150, 200), sf::Vector2f(1500, 800));

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
							const Segment<sf::Vector2f> s(nodes[i], nodes[i + 1]);
							const Segment<sf::Vector2f>::Projection p = s.getProjectionOf(current_point);
							if (p.k >= 0.0f && p.k <= s.size) {
								const float dist = Utils::getLength(p.point - current_point);
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
				const float length = Utils::getLength(to_last_point);

				if (length > current_length) {
					branches.back().nodes.emplace_back(last_point + current_length * Utils::getNormalized(to_last_point));
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
						const float length = Utils::getLength(vec);
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
					const Segment<sf::Vector2f> s(nodes[i], nodes[i+1]);
					const Segment<sf::Vector2f>::Projection p = s.getProjectionOf(current_point);
					if (p.k >= 0.0f && p.k <= s.size) {
						const float dist = Utils::getLength(p.point - current_point);
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
