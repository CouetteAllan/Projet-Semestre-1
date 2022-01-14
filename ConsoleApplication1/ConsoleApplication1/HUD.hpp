#pragma once
#include "Game.hpp"

class sf::Sprite;

class HUD
{
public:

	static std::vector<sf::Text>		texts;
	
	static sf::Sprite	playerHP;

	static void init();
	static void draw(sf::RenderWindow& win);
	static void update(double dt);

};

