#include "Game.hpp"
#include <time.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Tool.hpp"
using namespace ImGui;


int main()
{
	sf::RenderWindow window(sf::VideoMode(Game::W, Game::H), "Jeu du turfu tavu");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(144);

	ImGui::SFML::Init(window);

	RectangleShape* playerShape = new RectangleShape(Vector2f(20, 45));
	playerShape->setFillColor(Color::Blue);
	playerShape->setOrigin(10, 45.0f/2.0f);
	
	RectangleShape* enemyShape = new RectangleShape(Vector2f(20, 45));
	enemyShape->setFillColor(Color::Blue);
	enemyShape->setOrigin(10, 45);

	Game::player = new PlayerEntity(playerShape, 20, 15);


	


	double tStart = getTimeStamp();
	double tEnterFrame = getTimeStamp();
	double tExitFrame = getTimeStamp();

	bool mouseLeftWasPressed = false;
	Vector2f mousePos;
	//bool enterWasPressed = false;
	BulletEntity* bullets = new BulletEntity();

	EnemyEntity* enemy = new EnemyEntity(enemyShape, 10, 20);

	Audio audio;

	World data;
	data.objects.push_back((Entity*)Game::player);
	data.objects.push_back((Entity*)bullets);
	data.objects.push_back((Entity*)enemy);

	sf::Vector2i winPos = window.getPosition();

	bool pause = false;
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
		if (pause)
			dt = 0;
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
				if (event.key.code == sf::Keyboard::Space)
					pause = !pause;
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

		if (mouseLeftIsPressed && timerFinished && enable && Game::player->alive) {
			auto pos = Game::player->getPosition();
			auto dir = mousePos - pos;
			float dirLen = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			sf::Vector2f dxy(1, 0);
			if (dirLen) {
				dxy = dir / dirLen;
			}
			dxy *= 20.0f;
			bullets->create(pos.x, pos.y, dxy.x * 2, dxy.y * 2);
			audio.laserShoot.play();
			timer.restart();
			Game::player->click = 1;
		}



		if (mouseLeftIsPressed)
			mouseLeftWasPressed = true;
		else
			mouseLeftWasPressed = false;





		ImGui::SFML::Update(window, clockImgui.restart());

		Game::im();

		Begin("Gun");
		SliderFloat("FireRate", &fireRate, 0.0f, 10.0f);
		Checkbox("Enable", &enable);
		End();
			
		Begin("Mouse");
		Value("Coord X Mouse", mousePos.x);
		Value("Coord Y Mouse", mousePos.y);
		End();


		Game::player->setMousePos(mousePos);

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

		if (Game::shake > 0) {
			int flip1 = (rand() % 2 == 0) ? 1 : -1;
			int flip2 = (rand() % 2 == 0) ? 1 : -1;
			window.setPosition(winPos + sf::Vector2i(0 + rand() % 10 * flip1, 0 + rand() % 8 * flip2));
		}
		else
			window.setPosition(winPos);
		Game::shake--;


		//ui

		ImGui::SFML::Render(window);
		window.display();
		tExitFrame = getTimeStamp();
	}
	ImGui::SFML::Shutdown();

	return 0;
}