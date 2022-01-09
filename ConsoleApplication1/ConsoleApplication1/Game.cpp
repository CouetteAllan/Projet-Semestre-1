#include "Game.hpp"
std::vector<sf::Vector2i> Game::walls;


void Game::init() {
	walls.push_back(sf::Vector2i(5, 5));
}

/*void Game::im() {
	using namespace ImGui;
	static bool imOpened = true;
	if (Begin("Game", &imOpened)) {
		if (player) player->im();

	}
	End();
}*/


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
