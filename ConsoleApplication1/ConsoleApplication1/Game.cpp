#include "Game.hpp"
int Game::score = 0;
int Game::shake = 0;
Particle Game::parts;
std::vector<sf::Vector2i> Game::walls;
PlayerEntity * Game::player = nullptr;
Sound Audio::laserShoot;
Music Audio::bgm;
Sound Audio::explosion;
Sound Audio::gameOver;
Sound Audio::hit;
Sound Audio::heal;


void Game::particlesAt(sf::Vector2f pos) {
	int flip1 = (rand() % 2 == 0) ? 1 : -1;
	int flip2 = (rand() % 2 == 0) ? 1 : -1;
	parts.create(pos.x, pos.y, flip1 * rand() % 200, flip2 * (rand() % 160) - 35);
}

void Game::init() {
	walls.push_back(sf::Vector2i(5, 5));
}

void Game::im() {
	using namespace ImGui;
	static bool imOpened = true;
	if (Begin("Game", &imOpened)) {
		if (player) player->im();

	}
	End();
}


static bool isColliding(int ccx, int ccy) {
	if (ccx < 0)
		return true;
	if (ccy < 0)
		return true;

	if (ccx >= 1280 / Entity::stride)
		return true;

	if (ccy >= 720 / Entity::stride)
		return true;

	for (auto& vi : Game::walls)
		if ((vi.x == ccx) && (vi.y == ccy))
			return true;

	return false;
}

Audio::Audio()
{
	if (laserShootBuffer.loadFromFile("res/laserShoot.wav")) {

		Audio::laserShoot.setBuffer(laserShootBuffer);	
		Audio::laserShoot.setVolume(30.0f);
	}
	if (explosionBuffer.loadFromFile("res/explosion.wav"))
		Audio::explosion.setBuffer(explosionBuffer);

	if (gameOverBuffer.loadFromFile("res/gameOver.wav"))
		Audio::gameOver.setBuffer(gameOverBuffer);
	
	if (hitBuffer.loadFromFile("res/impactSound.wav"))
		Audio::hit.setBuffer(hitBuffer);
	
	if (healBuffer.loadFromFile("res/heal.wav"))
		Audio::heal.setBuffer(healBuffer);

	if (bgm.openFromFile("res/bgm.wav")) {
		Audio::bgm.play();
		Audio::bgm.setVolume(8.5f);
		Audio::bgm.setLoop(true);
	}

}
