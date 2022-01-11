#include "Game.hpp"
int Game::score = 0;
std::vector<sf::Vector2i> Game::walls;
PlayerEntity * Game::player = nullptr;
Sound Audio::laserShoot;
Music Audio::bgm;
Sound Audio::hit;
Sound Audio::smallLaser;
SoundBuffer Audio::laserShootBuffer;
SoundBuffer Audio::hitBuffer;

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
	if (hitBuffer.loadFromFile("res/impactSound.wav"))
		Audio::hit.setBuffer(hitBuffer);
	if (!bgm.openFromFile("res/bgm.wav"))
		printf("isqjdfv");

	Audio::bgm.play();
	Audio::bgm.setVolume(8.0f);
	Audio::bgm.setLoop(true);

}
