#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>

const float PI = 3.141592653;


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

	float angle = 0.0;

	while (window.isOpen())
	{
		angle += 0.01f;
        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);


        window.display();
    }

    return 0;
}
