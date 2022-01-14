#include "HUD.hpp"
std::vector<sf::Text>		HUD::texts;
Sprite HUD::playerHP;
void HUD::init()
{
	playerHP.setTexture(PlayerEntity::text);
	playerHP.setTextureRect(sf::IntRect(0, 0, 128, 128));
	playerHP.setOrigin(64, 54);
	playerHP.setScale(0.4f, 0.4f);

}


void HUD::draw(sf::RenderWindow & win)
{

	if (Game::player->alive) {

		for (size_t i = 0; i < Game::player->HP; i++)
		{
			auto pos = Vector2f(1000 + (i * 50), 700);
			playerHP.setPosition(pos);
			win.draw(playerHP);
		}
	}
}

void HUD::update(double dt)
{


}
