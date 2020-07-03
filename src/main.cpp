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
	settings.antialiasingLevel = 8;
	
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	float time = 0.0f;
	bool wind = false;

	Solver solver;

	VerletPoint::ptr pt1 = solver.createPoint(WinWidth * 0.5f, 10.0f, 1.0f, false);
	VerletPoint::ptr pt2 = solver.createPoint(WinWidth * 0.5f, 20.0f, 1.0f);
	VerletPoint::ptr pt3 = solver.createPoint(WinWidth * 0.5f, 30.0f, 1.0f);
	
	Link::ptr link_1 = solver.createLink(pt1, pt2);
	Link::ptr link_2 = solver.createLink(pt2, pt3);

	Join::ptr jon = Join::create(link_1, link_2);

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
		}

		solver.update();

		window.clear(sf::Color::Black);

		solver.render(window);

        window.display();
    }

    return 0;
}
