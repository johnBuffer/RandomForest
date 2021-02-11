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
#include "tree_builder.hpp"

#include "tree_2.hpp"
#include "tree_builder_2.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(false);

	TreeConf tree_conf{
		50.0f, // branch_width
		0.95f, // branch_width_ratio
		0.75f, // split_width_ratio
		0.5f, // deviation
		PI * 0.25f, // split angle
		0.1f, // branch_split_var;
		20.0f, // branch_length;
		0.97f, // branch_length_ratio;
		0.5f, // branch_split_proba;
		0.0f, // double split
		Vec2(0.0f, -0.5f), // Attraction
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

	std::vector<sf::VertexArray> branches_va;
	v2::Tree tree_fast = v2::TreeBuilder::build(Vec2(WinWidth * 0.5f, WinHeight), tree_conf);

	LayerRenderer renderer(window);

	float base_wind_force = 0.05f;
	float max_wind_force = 30.0f;
	float current_wind_force = 0.0f;

	const float wind_force = 1.0f;
	std::vector<Wind> wind{
		Wind(2.0f * layer_conf.width, base_wind_force * wind_force, 0.0f, layer_conf.width),
		Wind(100.0f, 3.f * wind_force, 150.0f),
		Wind(300.0f, 2.f * wind_force, 250.0f),
		Wind(85.0f, 3.f * wind_force, 108.0f),
		Wind(500.0f, 4.f * wind_force, 400.0f),
	};

	const float dt = 0.016f;

	float time_sum_1 = 0.0f;
	float time_sum_2 = 0.0f;
	float img_count = 1.0f;

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
					tree_fast = v2::TreeBuilder::build(Vec2(WinWidth * 0.5f, WinHeight), tree_conf);
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

		for (Wind& w : wind) {
			w.update(dt, layer_conf.width);

			for (v2::Branch& b : tree_fast.branches) {
				if (w.isOver(b.segment.moving_point.position)) {
					b.segment.moving_point.acceleration += Vec2(1.0f, 0.0f) * w.strength;
				}
			}
		}

		if (boosting) {
			for (v2::Branch& b : tree_fast.branches) {
				b.segment.moving_point.acceleration += Vec2(1.0f, 0.0f) * wind_force;
			}
		}

		sf::Clock tree_2_clock;
		tree_fast.update(dt);
		const float elapsed_2 = tree_2_clock.getElapsedTime().asMicroseconds();
		time_sum_2 += elapsed_2;
		//std::cout << "Tree 2 update: " << time_sum_2 / img_count << "us" << std::endl;

		window.clear(sf::Color::Black);

		TreeRenderer::generateRenderData(tree_fast, branches_va);
		for (const auto& va : branches_va) {
			window.draw(va);
		}

		/*sf::VertexArray va_debug(sf::Lines, 2 * tree.branches.size());
		uint32_t i = 0;
		for (const v2::Branch& b : tree.branches) {
			va_debug[2 * i + 0].position = sf::Vector2f(b.segment.attach_point.x, b.segment.attach_point.y);
			va_debug[2 * i + 1].position = sf::Vector2f(b.segment.moving_point.x, b.segment.moving_point.y);
			va_debug[2 * i + 0].color = sf::Color::Red;
			va_debug[2 * i + 1].color = sf::Color::Red;
			++i;
		}
		window.draw(va_debug);*/

		/*i = 0;
		for (const v2::Branch& b : tree.branches) {
			const float length = b.joint_strength * 0.1f;
			sf::Vector2f bot(b.nodes.back().position.x, b.nodes.back().position.y);
			sf::Vector2f top(bot + length * sf::Vector2f(b.target_direction.x, b.target_direction.y));
			va_debug[2 * i + 0].position = bot;
			va_debug[2 * i + 1].position = top;
			va_debug[2 * i + 0].color = sf::Color::Green;
			va_debug[2 * i + 1].color = sf::Color::Green;
			++i;
		}
		window.draw(va_debug);*/

		img_count += 1.0f;

        window.display();
    }

    return 0;
}
