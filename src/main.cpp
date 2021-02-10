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
#include "layer.hpp"
#include "tree_2.hpp"
#include "tree_builder.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1920;
	constexpr uint32_t WinHeight = 1080;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Fullscreen, settings);
	window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(false);

	TreeConf tree_conf{
		80.0f, // branch_width
		0.95f, // branch_width_ratio
		0.5f, // split_width_ratio
		0.5f, // deviation
		PI * 0.25f, // split angle
		0.1f, // branch_split_var;
		35.0f, // branch_length;
		0.99f, // branch_length_ratio;
		0.5f, // branch_split_proba;
		0.0f, // double split
		Vec2(0.0f, -2.5f), // Attraction
		8
	};

	LayerConf layer_conf{
		2,
		2500.0f,
		1080.0f,
		1200.0f,
		tree_conf
	};

	const uint32_t layers_count = 12;
	int32_t current_last = 0;
	std::vector<Layer> layers;
	const float layer_space = 2.0f;
	for (uint32_t i(layers_count); i--;) {
		layers.emplace_back(layer_conf, float(i) * layer_space);
		layers.back().init();
	}

	LayerRenderer renderer(window);

	float base_wind_force = 0.5f;
	float max_wind_force = 30.0f;
	float current_wind_force = 0.0f;

	const float wind_force = 0.1f;
	std::vector<Wind> wind{
		Wind(2.0f * layer_conf.width, base_wind_force * wind_force, 0.0f, layer_conf.width),
		Wind(1000.0f, 3.f * wind_force, 1500.0f),
		Wind(300.0f, 2.f * wind_force, 250.0f),
		Wind(850.0f, 3.f * wind_force, 1080.0f),
		Wind(500.0f, 8.f * wind_force, 400.0f),
	};

	const float dt = 0.016f;

	bool boosting = false;

	v2::PhysicSegment segment(Vec2(WinWidth * 0.5f, WinHeight * 0.5f), Vec2(WinWidth * 0.5f, WinHeight * 0.5f) + Vec2(100.0f, 0.0f));

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
					for (Wind& w : wind) {
						w.strength *= 200.0f;
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
				else if (event.key.code == sf::Keyboard::Up) {
					for (Wind& w : wind) {
						w.strength *= 1.2f;
					}
				}
				else if (event.key.code == sf::Keyboard::Down) {
					for (Wind& w : wind) {
						w.strength /= 1.2f;
					}
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

		for (Wind& w : wind) {
			w.update(dt, layer_conf.width);
		}

		segment.update(dt);

		window.clear(sf::Color::Black);

		sf::VertexArray va(sf::Lines, 2);
		va[0].position = sf::Vector2f(segment.attach_point.x, segment.attach_point.y);
		va[1].position = sf::Vector2f(segment.moving_point.x, segment.moving_point.y);
		window.draw(va);

        window.display();
    }

    return 0;
}
