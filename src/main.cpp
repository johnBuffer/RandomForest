#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>
#include "solver.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::ContextSettings settings;
	//settings.antialiasingLevel = 8;
	
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	float time = 0.0f;
	bool wind = false;

	Solver solver;

	const float link_length = 40.0f;
	const uint32_t points_count = 5;
	VerletPoint::ptr last_point = solver.createPoint(WinWidth * 0.5f, WinHeight - 100.0f, 1.0f, false);
	Link::ptr last_link = nullptr;
	for (uint32_t i(0); i < points_count; ++i) {
		VerletPoint::ptr new_point = solver.createPoint(last_point->coords.x, last_point->coords.y - link_length, 1.0f);
		Link::ptr new_link = solver.createLink(last_point, new_point);
		last_point = new_point;
		if (i > 0 && i < 40) {
			solver.createJoin(last_link, new_link, 0.0f);
		}
		last_link = new_link;
	}

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

		solver.update();

		window.clear(sf::Color::Black);

		solver.render(window);

        window.display();
    }

    return 0;
}
