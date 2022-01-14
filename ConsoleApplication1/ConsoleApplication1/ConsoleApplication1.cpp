#include "Game.hpp"
#include <time.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Tool.hpp"
using namespace ImGui;


bool startGame() {
	sf::RenderWindow window(sf::VideoMode(Game::W, Game::H), "Jeu du turfu tavu");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(144);

	sf::Font fArial;
	if (!fArial.loadFromFile("res/04B_20__.TTF"))
		cout << "font not loaded" << endl;
	sf::Text textScore;
	textScore.setFont(fArial);
	textScore.setFillColor(sf::Color::White);
	textScore.setCharacterSize(35);
	textScore.setPosition(Vector2f(Game::W / 2, 20));
	textScore.setOrigin(250, 10);
	textScore.setFillColor(Color(17, 29, 158));

	HUD::texts.push_back(textScore);
	
	sf::Text gameOverText;
	gameOverText.setFont(fArial);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setCharacterSize(45);
	gameOverText.setPosition(Vector2f(Game::W / 2 - 320.0f, Game::H/2));
	gameOverText.setOrigin(250, 10);
	gameOverText.setString("		GAME OVER! \n Press \"R\" to restart");
	
	sf::Text waveText;
	waveText.setFont(fArial);
	waveText.setFillColor(sf::Color::White);
	waveText.setCharacterSize(25);
	waveText.setPosition(Vector2f(280 , 680));
	waveText.setOrigin(250, 10);
	waveText.setString("Wave : " + to_string(SpawnerEnemy::wavesEnemy));
	//ImGui::SFML::Init(window);

	RectangleShape* playerShape = new RectangleShape(Vector2f(20, 45));
	playerShape->setFillColor(Color::Blue);
	playerShape->setOrigin(10, 45.0f / 2.0f);


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

	HUD::init();

	sf::Vector2i winPos = window.getPosition();
	sf::Time  sec = sf::seconds(3.0f);
	bool pause = false;

	int nbEnemySpawned = 0;
	int enemyNbr = 2;
	int wavesEnemy = 1;
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
			//ImGui::SFML::ProcessEvent(event);//Intégration IMGUI
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


		bool mouseLeftIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool mouseIsReleased = (!mouseLeftIsPressed && mouseLeftWasPressed);
		mousePos = (Vector2f)Mouse::getPosition(window);

		bool timerFinished = timerAttack.getElapsedTime() >= sf::seconds(1.0f) / fireRate;
		bool timerSpawnFinished = timerSpawn.getElapsedTime() >= sec;
		if (!pause) {

			if (timerSpawnFinished) {
				int randomCX = rand() % (Game::W / Entity::stride) + 1;
				int randomCY = rand() % (Game::H / Entity::stride) + 1;

				spawn.spawnAtLocation(randomCX, randomCY);

				for (size_t i = 1; i < enemyNbr; i++)
				{
					int randomCX = rand() % (Game::W / Entity::stride) + 1;
					int randomCY = rand() % (Game::H / Entity::stride) + 1;
					spawn.spawnAtLocation(randomCX, randomCY);
				}
				nbEnemySpawned++;


				if (nbEnemySpawned == 5) {
					enemyNbr++;
					SpawnerEnemy::numberOfEnemiesMax++;
					nbEnemySpawned = 0;
					sec += sf::seconds(0.7f);
					SpawnerEnemy::wavesEnemy++;
				}

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

		}




		//ImGui::SFML::Update(window, clockImgui.restart());

		//Game::im();

		/*Begin("Gun");
		SliderFloat("FireRate", &fireRate, 0.0f, 10.0f);
		Checkbox("Enable", &enable);
		End();

		Begin("Mouse");
		Value("Coord X Mouse", mousePos.x);
		Value("Coord Y Mouse", mousePos.y);
		End();
		*/

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
		if (!pause)
			World::update(dt);
		textScore.setString("Score: " + to_string(Game::score));
		waveText.setString("Wave : " + to_string(SpawnerEnemy::wavesEnemy));
		////////////////////
		//DRAW
		World::draw(window);
		window.draw(textScore);
		window.draw(waveText);
		if (!Game::player->alive)
			window.draw(gameOverText);

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

		//ImGui::SFML::Render(window);
		window.display();
		tExitFrame = getTimeStamp();
		if (sf::Keyboard::isKeyPressed(Keyboard::R))
			return true;
		
		
	}
	//ImGui::SFML::Shutdown();
}


void runningGame() {
	if (startGame()) {
		Game::player = nullptr;
		World::objects.clear();
		SpawnerEnemy::numberOfEnemies = 0;
		SpawnerEnemy::numberOfEnemiesMax = 8;
		SpawnerEnemy::wavesEnemy = 1;
		Game::score = 0;
		runningGame();
	}
}

int main()
{
	runningGame();
	return 0;
}