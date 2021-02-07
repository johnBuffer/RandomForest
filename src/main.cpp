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

	float base_wind_force = 0.0f;
	std::vector<Wind> wind{
		Wind(WinWidth, base_wind_force, 0.0f, WinWidth),
		Wind(200.0f, 3.f, 500.0f),
		Wind(100.0f, 2.f, 250.0f),
		Wind(150.0f, 2.f, 180.0f),
	};

	const float dt = 0.016f;

	const float update_delay = 0.02f;
	uint64_t last_nodes_count = 0;
	bool growing = true;
	bool leaf = false;
	bool done = false;

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
					wind[0].strength = base_wind_force;
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					
				}
				else if (event.key.code == sf::Keyboard::W) {
					base_wind_force = 1.0f;
				}
				else {
					wind[0].strength = 30.0f;
				}
			}
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

		if (done) {
			for (Wind& w : wind) {
				w.update(dt, WinWidth);
			}

			for (Wind& w : wind) {
				for (Tree& t : trees) {
					for (PinnedSegment& p : t.segments) {
						if (w.isOver(p.particule.position)) {
							p.particule.acceleration += Vec2(1.0f, RNGf::getRange(1.0f)) * w.strength;
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

        window.display();
    }

    return 0;
}
