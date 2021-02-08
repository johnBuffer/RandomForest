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

#include <swarm.hpp>


int main()
{
    constexpr uint32_t WinWidth = 1920;
	constexpr uint32_t WinHeight = 1080;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Fullscreen, settings);
	//window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(false);

	TreeConf tree_conf{
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

	LayerConf layer_conf{
		4,
		1920.0f,
		1080.0f,
		500.0f,
		tree_conf
	};

	const uint32_t layers_count = 16;
	std::vector<Layer> layers;
	for (uint32_t i(layers_count); i--;) {
		layers.emplace_back(layer_conf);
		layers.back().init();
	}

	LayerRenderer renderer(window);

	float base_wind_force = 0.0f;
	float max_wind_force = 30.0f;
	float current_wind_force = 0.0f;

	std::vector<Wind> wind{
		Wind(WinWidth, base_wind_force, 0.0f, WinWidth),
		Wind(200.0f, 3.f, 500.0f),
		Wind(100.0f, 2.f, 250.0f),
		Wind(150.0f, 2.f, 180.0f),
	};

	const float dt = 0.016f;

	bool boosting = false;

	swrm::Swarm swarm(16);

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

		for (Wind& w : wind) {
			w.update(dt, WinWidth);
		}
		
		swrm::WorkGroup group = swarm.execute([&](uint32_t id, uint32_t group_size) {
			layers[id].update(dt, wind);
			layers[id].generateRenderArrays();
		}, 16);
		group.waitExecutionDone();

		window.clear(sf::Color::Black);

		uint32_t layer_i(0);
		for (const Layer& l : layers) {
			sf::RenderStates states;
			states.transform.translate(WinWidth * 0.25f * (layer_i % 4), WinHeight * 0.25f * (layer_i / 4));
			states.transform.scale(0.25f, 0.25f);
			renderer.render(l.render_data, states);
			++layer_i;
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
