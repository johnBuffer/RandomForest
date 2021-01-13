#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>

#include "solver.hpp"
#include "mouse_controller.hpp"
#include "physic_renderer.hpp"


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	Solver solver;
	MouseController mouse_controller(solver);
	PhysicRenderer renderer(solver);

	Index<Node> p1 = solver.addNode(800.0f, 800.0f);
	Index<Node> p2 = solver.addNode(800.0f, 700.0f);
	Index<Node> p3 = solver.addNode(800.0f, 600.0f);
	Index<Node> p4 = solver.addNode(800.0f, 500.0f);
	Index<Node> p5 = solver.addNode(800.0f, 400.0f);

	p1->is_static = true;
	p2->is_static = true;

	Index<Segment> s1 = solver.addSegment(p1, p2);
	Index<Segment> s2 = solver.addSegment(p2, p3);
	Index<Segment> s3 = solver.addSegment(p3, p4);
	Index<Segment> s4 = solver.addSegment(p4, p5);

	Index<Joint> j1 = solver.addJoint(s1, s2, 0.0f, 30.0f);
	Index<Joint> j2 = solver.addJoint(s2, s3, 0.0f, 30.0f);
	Index<Joint> j3 = solver.addJoint(s3, s4, 0.0f, 30.0f);

	float time = 0.0f;
	const float dt = 0.016f;

	bool clicking = false;
	Node* selected = nullptr;
	
	while (window.isOpen())
	{
		const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

		time += dt;
        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				clicking = true;
				selected = mouse_controller.getSelected(mouse_pos);
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				clicking = false;
				selected = nullptr;
			}
		}

		solver.update(dt);

		if (selected) {
			selected->position = MouseController::vec2i_to_vec2f(mouse_pos);
		}

		window.clear(sf::Color::Black);

		renderer.render(window);

        window.display();
    }

    return 0;
}
