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
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Jeu du turfu tavu");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(144);

	ImGui::SFML::Init(window);

	RectangleShape* playerShape = new RectangleShape(Vector2f(20, 45));
	playerShape->setFillColor(Color::Blue);

	PlayerEntity* player = new PlayerEntity(playerShape, 20, 15);


	


	double tStart = getTimeStamp();
	double tEnterFrame = getTimeStamp();
	double tExitFrame = getTimeStamp();


	bool mouseLeftWasPressed = false;
	Vector2f mousePos;
	//bool enterWasPressed = false;
	BulletEntity bullets;

	World data;
	data.objects.push_back((Entity*)player);
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			player->dy -= 2 * player->speedMultiplier;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player->dy += 2 * player->speedMultiplier;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player->dx += 2 * player->speedMultiplier;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			player->dx -= 2 * player->speedMultiplier;

		}


		
		bool mouseLeftIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool mouseIsReleased = (!mouseLeftIsPressed && mouseLeftWasPressed);
		mousePos = (Vector2f)Mouse::getPosition(window);

		if (mouseLeftIsPressed) {
			auto pos = player->getPosition();
			auto dir = mousePos - pos;
			float dirLen = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			sf::Vector2f dxy(1, 0);
			if (dirLen) {
				dxy = dir / dirLen;
			}
			dxy *= 60.0f * 3;
			bullets.create(pos.x, pos.y, dxy.x, dxy.y);
		}


		if (mouseLeftIsPressed)
			mouseLeftWasPressed = true;
		else
			mouseLeftWasPressed = false;

		ImGui::SFML::Update(window, clock.restart());

		Begin("Coordonates");
		static bool modified;
		modified = SliderInt("CX", &player->cx, 0.0f, wWidth / player->stride);
		modified = SliderInt("CY", &player->cy, 0.0f, wHeight / player->stride - 2);
		modified = SliderFloat("RX", &player->rx, 0.0f, 1.0f);
		modified = SliderFloat("RY", &player->ry, 0.0f, 1.0f);
		modified = Checkbox("Enable Gravity", &player->gravity);

		Value("Coord X", player->sprite->getPosition().x);
		Value("Coord Y", player->sprite->getPosition().y);
		Value("Speed X", player->dx);
		Value("Speed Y", player->dy);
		Value("IsGrounded", player->isGrounded);
		Value("Mouse Position X", mousePos.x);
		Value("Mouse Position Y", mousePos.y);

		SliderFloat("Friction", &player->friction, 0.0f, 1.0f);
		if (modified)
			player->syncSprite();
		End();
			

		

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