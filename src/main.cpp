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

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	Solver solver;

	Tree tree1;
	tree1.branch_length = 50.0f;
	tree1.branch_length_ratio = 0.95f;
	tree1.fork_amplitude = 0.25f * PI;
	tree1.branch_distortion = PI * 0.125f;
	tree1.fork_probability = 0.25f;

	float current_position = 500.0f;

	float time = 0.0f;
	bool wind = false;

	/*VerletPoint::ptr p1 = VerletPoint::create(800, 450, 1.0f);
	VerletPoint::ptr p2 = VerletPoint::create(750, 350, 1.0f);
	VerletPoint::ptr p3 = VerletPoint::create(750, 250, 1.0f);
	VerletPoint::ptr p4 = VerletPoint::create(750, 150, 1.0f);
	VerletPoint::ptr p5 = VerletPoint::create(650, 150, 1.0f);
	VerletPoint::ptr p6 = VerletPoint::create(650, 150, 1.0f);
	VerletPoint::ptr p7 = VerletPoint::create(650, 150, 1.0f);
	VerletPoint::ptr p8 = VerletPoint::create(650, 150, 1.0f);

	p1->moving = false;

	solver.points.push_back(p1);
	solver.points.push_back(p2);
	solver.points.push_back(p3);
	solver.points.push_back(p4);
	solver.points.push_back(p5);
	solver.points.push_back(p6);
	solver.points.push_back(p7);
	solver.points.push_back(p8);

	solver.joins.push_back(Join(nullptr, p1, p2, PI*0.5f, 100.0f, 0.00002f));
	solver.joins.push_back(Join(p1, p2, p3, PI*0.25f, 100.0f, 0.2f));
	solver.joins.push_back(Join(p2, p3, p4, -PI * 0.25f, 100.0f, 0.2f));
	solver.joins.push_back(Join(p3, p4, p5, 0.25f, 100.0f, 0.2f));
	solver.joins.push_back(Join(p4, p5, p6, 0.0f, 100.0f, 0.2f));
	solver.joins.push_back(Join(p5, p6, p7, 0.0f, 100.0f, 0.02f));
	solver.joins.push_back(Join(p6, p7, p8, 0.0f, 100.0f, 0.02f));*/

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
					tree1.create(solver, 16, 800, 600);
				}
				else {
					wind = !wind;
				}
			}
		}

		if (wind) {
			const float force = 5000.0f;
			const float wind_t = time * 0.2f;
			const float wind_intensity_x = 100.0f * sin(wind_t) * sin(wind_t) + rand() % 30 - 15;
			const float wind_intensity_y = 100.0f * sin(2.0f * wind_t) + rand() % 10 - 5;
			for (VerletPoint::ptr pt : solver.points) {
				pt->acceleration = Vec2(wind_intensity_x, wind_intensity_y) * force;
			}
		}

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

		for (const VerletPoint::ptr point : solver.points) {
			const float r = 5.0f;
			sf::CircleShape c(r, r);
			c.setOrigin(r, r);
			c.setFillColor(sf::Color::Red);
			c.setPosition(point->coords.x, point->coords.y);
			window.draw(c);
		}

		for (uint32_t i(0); i < n_joins; ++i) {
			const Join& j = solver.joins[i];

			if (j.point2) {
				const float r = 4.0f;
				sf::CircleShape c(r, r);
				c.setOrigin(r, r);
				c.setFillColor(sf::Color::Yellow);
				c.setPosition(j.point2->coords.x, j.point2->coords.y);
				window.draw(c);
			}
		}

        window.display();
    }

    return 0;
}
