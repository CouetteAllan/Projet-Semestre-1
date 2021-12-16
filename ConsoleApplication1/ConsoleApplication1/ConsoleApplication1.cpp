#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Tool.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include "Entity.hpp"
#include "World.hpp"
#include <time.h>
#include <stdio.h>
#include <errno.h>

#include "imgui.h"
#include "imgui-SFML.h"

using namespace ImGui;



int main()
{
	float wHeight = 720;
	float wWidth = 1280;
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Moteur à Case");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(144);

	ImGui::SFML::Init(window);




	


	double tStart = getTimeStamp();
	double tEnterFrame = getTimeStamp();
	double tExitFrame = getTimeStamp();


	bool mouseLeftWasPressed = false;
	//bool enterWasPressed = false;
	World data;
	//----------------------------------------  IMGUI STUFF  -------------------------------------------------------------
	float bgCol[3] = { 0,0,0 };
	Clock clock;
	int click = 0;
	while (window.isOpen()) {
		sf::Event event;
		double dt = tExitFrame - tEnterFrame;
		tEnterFrame = getTimeStamp(); //calculer le temps entre chaque frame pour les vitesses
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);//Intégration IMGUI
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
		bool keyHit = false;




		
		bool mouseLeftIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool mouseIsReleased = (!mouseLeftIsPressed && mouseLeftWasPressed);
		




		if (mouseLeftIsPressed)
			mouseLeftWasPressed = true;
		else
			mouseLeftWasPressed = false;

		ImGui::SFML::Update(window, clock.restart());

		
			

		

		//ImGui::ShowDemoWindow(&activeTool);

		////////////////////
		//CLEAR
		window.clear(Color(
			bgCol[0] * 255,
			bgCol[1] * 255,
			bgCol[2] * 255
		));

		////////////////////
		//UPDATE
		data.update(dt);
		////////////////////
		//DRAW
		data.draw(window);
		//game elems



		//ui

		ImGui::SFML::Render(window);
		window.display();
		tExitFrame = getTimeStamp();
	}
	ImGui::SFML::Shutdown();

	return 0;
}