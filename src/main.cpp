#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>
#include "solver.hpp"
#include "grass.hpp"
#include "tree.hpp"
#include "wind.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	float time = 0.0f;
	bool wind = false;
	float wind_x = 0.0f;
	float wind_speed = 7.0f;
	float wind_width = 200.0f;
	float wind_force = 50.0f;

	std::vector<Wind> winds(4);

	Solver solver;

	for (float x(WinWidth*0.0f); x < WinWidth; x += 1.0f) {
		Grass::add(solver, x, WinHeight);
	}

	Tree::add(solver, WinWidth*0.5f, WinHeight - 100);

	VerletPoint::ptr selected_point = nullptr;

	while (window.isOpen())
	{
		auto mouse_pos = sf::Mouse::getPosition(window);
		//p2->moveTo(mouse_pos.x, mouse_pos.y);

		time += 0.008f;
        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::A) {
				}
				else {
					wind = !wind;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					selected_point = nullptr;
				}
				else {
					if (selected_point) {
						selected_point->moving = !selected_point->moving;
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					selected_point = solver.getPointAt(mouse_pos.x, mouse_pos.y);
				}
			}
		}

		if (selected_point) {
			selected_point->moveTo(mouse_pos.x, mouse_pos.y, true);
		}

		if (wind) {
			for (Wind& w : winds) {
				w.apply(solver);
				w.update(WinWidth);
			}
		}

		solver.update();

		window.clear(sf::Color::Black);

		solver.render(window);

        window.display();
    }

    return 0;
}
