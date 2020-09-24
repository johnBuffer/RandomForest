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

	const float branch_length = 100.0f;
	const float branch_length_ratio = 0.85f;
	const float branch_width = 0.5f;

	float current_length;

	std::vector<Branch> branches;

	sf::VertexArray va;

	while (window.isOpen())
	{
		auto mouse_pos = sf::Mouse::getPosition(window);

        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				clicking = true;
				current_length = branch_length;
				branches.emplace_back();
				branches.back().nodes.emplace_back(mouse_pos.x, mouse_pos.y);
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				clicking = false;
			}
		}

		if (!branches.empty()) {
			if (clicking) {
				const sf::Vector2f current_point = sf::Vector2f(mouse_pos.x, mouse_pos.y);
				const sf::Vector2f last_point = branches.back().nodes.back();
				sf::Vector2f to_last_point = current_point - last_point;
				const float length = getLength(to_last_point);

				if (length > current_length) {
					branches.back().nodes.emplace_back(last_point + current_length * getNormalized(to_last_point));
					current_length *= branch_length_ratio;
				}
			}

			uint64_t nodes_count = branches.back().nodes.size();
			va = sf::VertexArray(sf::TriangleStrip, 4 * nodes_count);
			if (nodes_count > 1) {
				for (uint64_t i(0); i < nodes_count - 1; ++i) {
					const sf::Vector2f vec = branches.back().nodes[i + 1] - branches.back().nodes[i];
					const float length = getLength(vec);
					const sf::Vector2f vec_normalized = vec / length;
					const sf::Vector2f vec_normal(-vec_normalized.y, vec_normalized.x);
					const float width = length * branch_length_ratio * 0.5f;

					va[4 * i + 0].position = branches.back().nodes[i] + width * vec_normal;
					va[4 * i + 1].position = branches.back().nodes[i] - width * vec_normal;
					va[4 * i + 2].position = branches.back().nodes[i + 1] + width * branch_length_ratio * vec_normal;
					va[4 * i + 3].position = branches.back().nodes[i + 1] - width * branch_length_ratio * vec_normal;
				}
			}
		}

		window.clear(sf::Color::Black);

		window.draw(va);

        window.display();
    }

    return 0;
}
