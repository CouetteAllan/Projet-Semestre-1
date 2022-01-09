#include "Game.hpp"
#include <time.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Tool.hpp"
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
	playerShape->setOrigin(10, 45);

	PlayerEntity* player = new PlayerEntity(playerShape, 20, 15);

	RectangleShape gun(sf::Vector2f(8, 32));
	gun.setFillColor(sf::Color(0xFF, 0x00, 0x00));
	gun.setOrigin(4, 10);

	


	double tStart = getTimeStamp();
	double tEnterFrame = getTimeStamp();
	double tExitFrame = getTimeStamp();

	bool mouseLeftWasPressed = false;
	Vector2f mousePos;
	//bool enterWasPressed = false;
	BulletEntity* bullets = new BulletEntity();

	World data;
	data.objects.push_back((Entity*)player);
	data.objects.push_back((Entity*)bullets);
	//----------------------------------------  IMGUI STUFF  -------------------------------------------------------------
	float bgCol[3] = { 0,0,0 };
	Clock clockImgui;

	//----------------------------------------  GUN   --------------------------------------------------------------------
	Clock timer;
	bool enable = true;
	static float fireRate = 5.0f;

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
/*
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
		}*/


		
		bool mouseLeftIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool mouseIsReleased = (!mouseLeftIsPressed && mouseLeftWasPressed);
		mousePos = (Vector2f)Mouse::getPosition(window);

		bool timerFinished = timer.getElapsedTime() >= sf::seconds(1.0f) / fireRate;

		if (mouseLeftIsPressed && timerFinished && enable) {
			auto pos = gun.getPosition();
			auto dir = mousePos - pos;
			float dirLen = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			sf::Vector2f dxy(1, 0);
			if (dirLen) {
				dxy = dir / dirLen;
			}
			dxy *= 20.0f;
			bullets->create(pos.x, pos.y, dxy.x * 2, dxy.y * 2);
			timer.restart();
			player->click = 1;
		}



		if (mouseLeftIsPressed)
			mouseLeftWasPressed = true;
		else
			mouseLeftWasPressed = false;





		ImGui::SFML::Update(window, clockImgui.restart());

		player->im();

		Begin("Gun");
		SliderFloat("FireRate", &fireRate, 0.0f, 10.0f);
		Checkbox("Enable", &enable);
		End();
			

		sf::Vector2f characterToMouse(
			mousePos.y - player->getPosition().y,
			mousePos.x - player->getPosition().x);

		float radToDeg = 57.2958;
		float angleC2M = atan2(characterToMouse.y, characterToMouse.x);
		gun.setRotation(-angleC2M * radToDeg);
		gun.setPosition(player->getPosition().x + 5, player->getPosition().y - 20);

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
		window.draw(gun);
		//game elems



		//ui

		ImGui::SFML::Render(window);
		window.display();
		tExitFrame = getTimeStamp();
	}
	ImGui::SFML::Shutdown();

	return 0;
}