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
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(4);

	Solver solver;

	Tree tree1;
	tree1.branch_length = 100.0f;
	tree1.branch_length_ratio = 0.85f;
	tree1.fork_amplitude = 0.75f * PI;
	tree1.branch_distortion = PI * 0.25f;
	tree1.fork_probability = 0.0f;

	float current_position = 500.0f;

	float time = 0.0f;
	bool wind = false;

	/*VerletPoint::ptr p1 = VerletPoint::create(800, 450, 1.0f);
	VerletPoint::ptr p2 = VerletPoint::create(800, 400, 1.0f);
	VerletPoint::ptr p3 = VerletPoint::create(750, 400, 1.0f);
	VerletPoint::ptr p4 = VerletPoint::create(800, 350, 1.0f);
	
	p1->moving = false;

	solver.points.push_back(p1);
	solver.points.push_back(p2);
	solver.points.push_back(p3);
	//solver.points.push_back(p4);
	
	solver.joins.push_back(Join(nullptr, p1, p2, PI*0.5f, 100.0f, 0.2f));
	solver.joins.push_back(Join(p1, p2, p3, -PI*0.5f, 100.0f, 0.2f));
	solver.joins.push_back(Join(p1, p2, p4, 0.0f, 100.0f, 0.2f));
	
	solver.links.push_back(Link(p1, p2));
	solver.links.push_back(Link(p2, p3));
	solver.links.push_back(Link(p2, p4));*/

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
					tree1.create(solver, 2, 800, 600);

				}
				else if (event.key.code == sf::Keyboard::W) {
					wind = !wind;
				}
			}
		}

		//p2->moveTo(rotate(p2->coords, 0.005f, p1->coords));

		if (wind) {
			const float force = 0.2f;
			const float wind_t = time * 0.2f;
			const float wind_intensity_x = 40 - getRandRange(50);
			const float wind_intensity_y = getRandRange(20);
			for (VerletPoint::ptr pt : solver.points) {
				pt->applyForce(Vec2(wind_intensity_x, wind_intensity_y) * force);
			}
		}

		solver.update(0.016f);

		const uint64_t n_joins = solver.joins.size();
		sf::VertexArray va(sf::Lines, 0);
		for (uint64_t i(0); i < n_joins; ++i) {
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
			const size_t r = 5;
			sf::CircleShape c(r, r);
			c.setOrigin(r, r);
			c.setFillColor(sf::Color::Red);
			c.setPosition(point->coords.x, point->coords.y);
			window.draw(c);
		}

		for (uint32_t i(0); i < n_joins; ++i) {
			const Join& j = solver.joins[i];

			if (j.point2) {
				const size_t r = 4;
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
