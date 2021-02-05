#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>

#include "tree.hpp"
#include "tree_renderer.hpp"
#include "mouse_controller.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	TreeConf conf = {
		30.0f, // branch_width
		0.9f, // branch_width_ratio
		0.65f, // split_width_ratio
		0.5f, // deviation
		0.7f, // split angle
		0.3f, // branch_split_var;
		20.0f, // branch_length;
		0.99f, // branch_length_ratio;
		0.5f, // branch_split_proba;
		0.0f, // double split
		Vec2(0.0f, -0.7f), // Attraction
		8
	};
	
	Tree tree(Vec2(WinWidth/2, WinHeight), conf);
	tree.fullGrow();

	TreeRenderer renderer(window);

	const float update_delay = 0.01f;

	const float angle = RNGf::getRange(3.0f);
	Leaf test_leaf(Vec2(WinWidth / 2, WinHeight / 2), Vec2(cos(angle), sin(angle)));

	sf::Texture texture;
	texture.loadFromFile("../res/leaf.png");

	std::vector<Wind> wind{
		Wind(200.0f, 3.f, 500.0f),
		Wind(500.0f, 2.f, 50.0f),
		Wind(100.0f, 2.f, 250.0f),
	};

	const float dt = 0.016f;

	sf::Clock clock;
	while (window.isOpen())
	{
		const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::KeyReleased) {
				tree = Tree(Vec2(WinWidth / 2, WinHeight), conf);
				tree.fullGrow();
			}
		}

		for (Wind& w : wind) {
			w.update(dt, WinWidth);
		}

		tree.update(dt, wind);

		window.clear(sf::Color::Black);

		renderer.render(tree);

		/*for (const Wind& w : wind) {
			sf::RectangleShape wind_debug(sf::Vector2f(w.width, WinHeight));
			wind_debug.setPosition(w.pos_x, 0.0f);
			wind_debug.setFillColor(sf::Color(255, 0, 0, 100));
			window.draw(wind_debug);
		}*/

        window.display();
    }

    return 0;
}
