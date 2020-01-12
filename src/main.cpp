#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>

#include "solver.hpp"
#include "tree.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	Solver solver;

	Tree tree1;
	tree1.branch_length = 100.0f;
	tree1.branch_length_ratio = 0.8f;
	tree1.fork_amplitude = PI;
	tree1.fork_count = 2U;

	tree1.create(solver, 10, 800, 600);

	float time = 0.0f;

	while (window.isOpen())
	{
		time += 0.008f;
        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyReleased) {
				//solver.update(0.008f);
			}
		}

		/*const float wind_intensity = rand()%1000;
		for (VerletPoint::ptr pt : solver.points) {
			pt->acceleration = Vec2(wind_intensity, 0.0f);
		}*/

		solver.update(0.008f);

		const uint32_t n_joins = solver.joins.size();
		sf::VertexArray va(sf::Lines, 0);
		for (uint32_t i(0); i < n_joins; ++i) {
			const Join& j = solver.joins[i];
			if (j.point2) {
				sf::Vertex vertex;
				vertex.position = sf::Vector2f(j.point2->coords.x, j.point2->coords.y);
				vertex.color = sf::Color::White;
				va.append(vertex);
			}

			if (j.point3) {
				sf::Vertex vertex;
				vertex.position = sf::Vector2f(j.point3->coords.x, j.point3->coords.y);
				vertex.color = sf::Color::White;
				va.append(vertex);
			}
		}

		window.clear(sf::Color::Black);

		window.draw(va);
		for (uint32_t i(0); i < n_joins; ++i) {
			const Join& j = solver.joins[i];

			if (j.point2) {
				const float r = 4.0f;
				sf::CircleShape c(r, r);
				c.setOrigin(r, r);
				c.setFillColor(sf::Color::Green);
				c.setPosition(j.point2->coords.x, j.point2->coords.y);
				window.draw(c);
			}
		}

        window.display();
    }

    return 0;
}
