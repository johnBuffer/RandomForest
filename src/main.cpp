#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>
#include "segment.hpp"
#include "utils.hpp"
#include "tree2.hpp"


int main()
{
	//srand(time(0));

    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	sf::Vector2i clic_pos;
	bool clicking = false;


	float current_length;

	Tree2 tree;

	while (window.isOpen())
	{
		auto mouse_pos = sf::Mouse::getPosition(window);
		const sf::Vector2f current_point = sf::Vector2f(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y));

        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);



        window.display();
    }

    return 0;
}
