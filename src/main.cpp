#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>

#include "tree.hpp"
#include "mouse_controller.hpp"
#include "grass/grass.hpp"
#include "gauge_bar.hpp"
#include "layer.hpp"
#include "tree_builder.hpp"

#include <swarm.hpp>


int main()
{
    constexpr uint32_t WinWidth = 1920;
	constexpr uint32_t WinHeight = 1080;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(false);

	v2::TreeConf tree_conf{
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

	const uint32_t layers_count = 2;
	int32_t current_last = 0;
	std::vector<Layer> layers;
	const float layer_space = 5.0f;
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

		for (Layer& layer : layers) {
			const float dist_threshold = 0.1f;
			if (layer.dist > dist_threshold) {
				layer.update(dt, wind);
				layer.generateRenderArrays(dist_threshold);
			}
			else {
				layer.dist = layers_count * layer_space;
				layer.back_to_end = true;
				layer.init();
			}
		}

		const float scroll_speed = 1.0f;
		for (Layer& l : layers) {
			//l.dist -= scroll_speed * dt;
			if (l.back_to_end) {
				--current_last;
				if (current_last < 0) {
					current_last += layers_count;
				}
				l.back_to_end = false;
			}
		}

		window.clear(sf::Color::Black);

		const sf::Vector2f mid_window(float(WinWidth) * 0.5f, float(WinHeight) * 0.65f);
		const sf::Vector2f mid_layer(layer_conf.width * 0.5f, layer_conf.height * 0.9f);
		const float max_depth = layers_count * layer_space;
		uint64_t layer_i(layers.size() - 1);
		
		for (int32_t i(0); i<layers_count; ++i) {
			int32_t index = (current_last + i) % layers_count;

			const uint32_t side_id = (index + 2) % layers_count;
			const Layer& l_side = layers[index];

			const Layer& l = layers[index];
			const float scale = 2.0f / (l.dist + 0.1f);

			sf::RenderStates states;
			states.transform.translate(mid_window);
			states.transform.scale(scale, scale);
			states.transform.translate(-mid_layer);
			// Left
			sf::RenderStates states_left = states;
			states_left.transform.translate(-layer_conf.width, 0.0f);
			renderer.render(l_side.render_data, states_left);
			// Mid
			renderer.render(l.render_data, states);
			// Right
			sf::RenderStates states_right = states;
			states_right.transform.translate(layer_conf.width, 0.0f);
			renderer.render(l_side.render_data, states_right);

			// Fake side id
			/*renderer.renderGrass(l_side.render_data, states_left);
			renderer.renderGrass(l.render_data, states);
			renderer.renderGrass(l_side.render_data, states_right);*/

			sf::VertexArray fade(sf::Quads, 4);
			fade[0].position = sf::Vector2f(0.0f, 0.0f);
			fade[1].position = sf::Vector2f(WinWidth, 0.0f);
			fade[2].position = sf::Vector2f(WinWidth, WinHeight);
			fade[3].position = sf::Vector2f(0.0f, WinHeight);

			fade[0].color = sf::Color(255, 171, 133, (100.0f * l.dist / max_depth));
			fade[1].color = sf::Color(255, 171, 133, (100.0f * l.dist / max_depth));
			fade[2].color = sf::Color(255, 255, 255, (100.0f * l.dist / max_depth));
			fade[3].color = sf::Color(255, 255, 255, (100.0f * l.dist / max_depth));

			window.draw(fade);
			--layer_i;
		}

        window.display();
    }

    return 0;
}
