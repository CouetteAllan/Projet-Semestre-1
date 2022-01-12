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
	

	Game::player = new PlayerEntity(playerShape, 20, 15);


	SpawnerEnemy spawn;


	double tStart = getTimeStamp();
	double tEnterFrame = getTimeStamp();
	double tExitFrame = getTimeStamp();

	bool mouseLeftWasPressed = false;
	Vector2f mousePos;
	//bool enterWasPressed = false;
	BulletEntity* bullets = new BulletEntity();

	srand(time(NULL));

	Audio audio;

	World::objects.push_back(Game::player);
	World::objects.push_back(bullets);

	spawn.spawnAtLocation(12, 13);
	spawn.spawnAtLocation(14, 2);
	spawn.spawnAtLocation(11, 2);
	spawn.spawnAtLocation(8, 2);

	sf::Vector2i winPos = window.getPosition();
	sf::Time  sec = sf::seconds(5.0f);
	bool pause = false;
	//----------------------------------------  IMGUI STUFF  -------------------------------------------------------------
	float bgCol[3] = { 0,0,0 };
	Clock clockImgui;

	//----------------------------------------  GUN   --------------------------------------------------------------------
	Clock timerAttack;
	Clock timerSpawn;
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

		bool timerFinished = timerAttack.getElapsedTime() >= sf::seconds(1.0f) / fireRate;
		bool timerSpawnFinished = timerSpawn.getElapsedTime() >= sec;

		if (timerSpawnFinished) {
			int randomCX = rand() % (Game::W / Entity::stride) + 1;
			int randomCY = rand() % (Game::W / Entity::stride) + 1;

			spawn.spawnAtLocation(randomCX, randomCY);
			timerSpawn.restart();
		}

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
			timerAttack.restart();
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
		World::update(dt);
		////////////////////
		//DRAW
		World::draw(window);
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