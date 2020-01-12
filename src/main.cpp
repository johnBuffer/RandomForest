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

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	Solver solver;

	Join* j0 = new Join(800, 450, -PI * 0.25f);
	Join* j1 = new Join(850, 350, 0.0f);
	Join* j2 = new Join(900, 250, 0.0f);

	j0->position.moving = false;
	j0->setConnection2(j1);
	j0->strength = 0.01f;
	j1->setConnection1(j0);
	j1->setConnection2(j2);
	j1->strength = 0.01f;
	j2->setConnection1(j1);

	j0->position.acceleration = Vec2(0.0f, 10000.0f);
	j1->position.acceleration = Vec2(0.0f, 10000.0f);
	j2->position.acceleration = Vec2(0.0f, 10000.0f);

	solver.joins.push_back(j0);
	solver.joins.push_back(j1);
	solver.joins.push_back(j2);


	while (window.isOpen())
	{
        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyReleased) {
				//solver.update(0.008f);
			}
		}

		solver.update(0.008f);

		const uint32_t n_joins = solver.joins.size();
		sf::VertexArray va(sf::Lines, 0);
		for (uint32_t i(0); i < n_joins; ++i) {
			const Join& j = *solver.joins[i];
			sf::Vertex vertex1;
			vertex1.position = sf::Vector2f(j.position.coords.x, j.position.coords.y);
			vertex1.color = sf::Color::White;
			va.append(vertex1);

			if (j.connected2) {
				sf::Vertex vertex2;
				vertex2.position = sf::Vector2f(j.connected2->position.coords.x, j.connected2->position.coords.y);
				vertex2.color = sf::Color::White;
				va.append(vertex2);
			}
		}

		window.clear(sf::Color::Black);

		window.draw(va);
		for (uint32_t i(0); i < n_joins; ++i) {
			const Join& j = *solver.joins[i];
			const float r = 4.0f;
			sf::CircleShape c(r, r);
			c.setOrigin(r, r);
			c.setFillColor(sf::Color::Green);
			c.setPosition(j.position.coords.x, j.position.coords.y);
			window.draw(c);
		}

        window.display();
    }

    return 0;
}
