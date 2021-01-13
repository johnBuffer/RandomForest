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

	Index<Segment> s1 = solver.addSegment(p1, p2);

	float time = 0.0f;

	bool clicking = false;
	Node* selected = nullptr;
	
	while (window.isOpen())
	{
		const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

		time += 0.008f;
        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				clicking = true;
				selected = mouse_controller.getSelected(mouse_pos);
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				clicking = true;
				selected = mouse_controller.getSelected(mouse_pos);
			}
		}

		if (selected) {
			selected->position = MouseController::vec2i_to_vec2f(mouse_pos);
		}

		window.clear(sf::Color::Black);

		renderer.render(window);

        window.display();
    }

    return 0;
}
