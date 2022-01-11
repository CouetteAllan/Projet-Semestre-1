#include "Game.hpp"
int Game::score = 0;
int Game::shake = 0;
Particle Game::parts;
std::vector<sf::Vector2i> Game::walls;
PlayerEntity * Game::player = nullptr;
Sound Audio::laserShoot;
Music Audio::bgm;
Sound Audio::explosion;
Sound Audio::smallLaser;
SoundBuffer Audio::laserShootBuffer;
SoundBuffer Audio::explosionBuffer;

void Game::particlesAt(sf::Vector2f pos) {
	int flip = (rand() % 2 == 0) ? 1 : -1;
	parts.create(pos.x, pos.y, flip * rand() % 200, -(rand() % 80) - 35);
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
	if (laserShootBuffer.loadFromFile("res/laserShoot.wav"))
		Audio::laserShoot.setBuffer(laserShootBuffer);
	if (explosionBuffer.loadFromFile("res/explosion.wav"))
		Audio::explosion.setBuffer(explosionBuffer);
	if (!bgm.openFromFile("res/bgm.wav"))
		printf("isqjdfv");

	Audio::bgm.play();
	Audio::bgm.setVolume(8.0f);
	Audio::bgm.setLoop(true);

}
