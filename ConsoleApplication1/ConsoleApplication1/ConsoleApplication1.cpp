#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include "imgui/imconfig-SFML.h"

#include "Tool.hpp"

int main()
{
	float wHeight = 920;
	float wWidth = 1580;
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Jeu de ouf");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(144);


	double tStart = getTimeStamp();
	double tEnterFrame = getTimeStamp();
	double tExitFrame = getTimeStamp();


	while (window.isOpen()) {
		sf::Event event;
		double dt = tExitFrame - tEnterFrame;
		tEnterFrame = getTimeStamp(); //calculer le temps entre chaque frame pour les vitesses
		while (window.pollEvent(event)) {
			switch (event.type)
			{

			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;

			default:
				break;
			}
		}


		window.clear();

		window.display();
		tExitFrame = getTimeStamp();
	}

	return 0;
}
