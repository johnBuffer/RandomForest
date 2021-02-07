#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>

#include "tree.hpp"
#include "tree_renderer.hpp"
#include "tree_debug_renderer.hpp"
#include "mouse_controller.hpp"
#include "grass/grass.hpp"
#include "gauge_bar.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1920;
	constexpr uint32_t WinHeight = 1080;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Fullscreen, settings);
	window.setFramerateLimit(60);

	TreeConf conf = {
		80.0f, // branch_width
		0.95f, // branch_width_ratio
		0.5f, // split_width_ratio
		0.25f, // deviation
		PI * 0.25f, // split angle
		0.1f, // branch_split_var;
		15.0f, // branch_length;
		0.99f, // branch_length_ratio;
		0.5f, // branch_split_proba;
		0.0f, // double split
		Vec2(0.0f, -0.5f), // Attraction
		8
	};

	const uint32_t trees_count = 1;
	std::vector<Tree> trees;
	for (uint32_t i(0); i < trees_count; ++i) {
		trees.emplace_back(Vec2(WinWidth*0.5f, WinHeight), conf);
		//trees.back().fullGrow();
	}

	TreeRenderer renderer(window);
	TreeDebugRenderer debug_renderer(window);

	sf::Texture texture;
	texture.loadFromFile("../res/leaf.png");
	sf::Font font;
	font.loadFromFile("../res/font_2.ttf");
	sf::Text text("Wind Force", font, 20);
	text.setPosition(20.0f, 50.0f);
	

	float base_wind_force = 0.0f;
	float max_wind_force = 30.0f;
	float current_wind_force = 0.0f;

	std::vector<Wind> wind{
		Wind(WinWidth, base_wind_force, 0.0f, WinWidth),
		Wind(200.0f, 3.f, 500.0f),
		Wind(100.0f, 2.f, 250.0f),
		Wind(150.0f, 2.f, 180.0f),
	};

	GaugeBar bar(max_wind_force, sf::Vector2f(20.0f, 20.0f), sf::Vector2f(200.0f, 30.0f));

	const float dt = 0.016f;

	const float update_delay = 0.05f;
	uint64_t last_nodes_count = 0;
	bool growing = true;
	bool leaf = false;
	bool done = false;
	bool boosting = false;
	bool wait_start = true;

	sf::Clock clock;
	while (window.isOpen())
	{
		const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Space) {
					trees.clear();
					for (uint32_t i(0); i < trees_count; ++i) {
						trees.emplace_back(Vec2(WinWidth*0.5f, WinHeight), conf);
						growing = true;
						leaf = false;
						done = false;
						//trees.back().fullGrow();
					}
				}
				else {
					boosting = false;
					wind[0].strength = base_wind_force;
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				else if (event.key.code == sf::Keyboard::W) {
					base_wind_force = 1.0f;
				}
				else {
					boosting = true;
					if (current_wind_force < max_wind_force) {
						current_wind_force += 0.1f;
					}
					wind[0].strength = current_wind_force;
				}
			}
		}

		if (!boosting) {
			current_wind_force += (base_wind_force - current_wind_force) * 0.1f;
		}

		Tree& current_tree = trees[0];
		if (growing) {
			if (clock.getElapsedTime().asSeconds() > update_delay) {
				clock.restart();
				current_tree.grow();
				const uint64_t current_count = current_tree.getNodesCount();
				if ((current_count == last_nodes_count) && last_nodes_count) {
					growing = false;
				}
				last_nodes_count = current_count;
			}
		}
		else if (!leaf) {
			current_tree.createSkeleton();
			current_tree.addLeafs();
			leaf = true;
		}
		else {
			bool all_grown = true;
			for (Leaf& l : current_tree.leafs) {
				if (l.size < 1.0f) {
					all_grown = false;
					l.size += 0.03f;
				}
			}
			if (all_grown) {
				done = true;
			}
		}

		bar.setValue(current_wind_force);

		if (done) {
			for (Wind& w : wind) {
				w.update(dt, WinWidth);
			}

			for (Wind& w : wind) {
				for (Tree& t : trees) {
					for (PinnedSegment& p : t.segments) {
						if (w.isOver(p.particule.position)) {
							p.particule.acceleration += Vec2(1.0f, RNGf::getRange(2.0f)) * w.strength;
						}
					}
				}
			}

			for (Tree& t : trees) {
				t.update(dt, wind);
			}
		}

		window.clear(sf::Color::Black);

		for (const Tree& t : trees) {
			renderer.render(t);
			//debug_renderer.render(t);
		}

		/*for (const Wind& w : wind) {
			sf::RectangleShape wind_debug(sf::Vector2f(w.width, WinHeight));
			wind_debug.setPosition(w.pos_x - w.width * 0.5f, 0.0f);
			wind_debug.setFillColor(sf::Color(255, 0, 0, 100));
			window.draw(wind_debug);
		}*/

		sf::RenderStates states;
		bar.render(window, states);
		window.draw(text);

        window.display();
    }

    return 0;
}
