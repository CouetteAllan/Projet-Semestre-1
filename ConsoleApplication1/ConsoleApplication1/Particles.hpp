#pragma once
#include "Game.hpp"

class sf::Sprite;
//class sf::Texture;

class Particle {
public:
	sf::Sprite part;
	sf::Texture particleText;
	std::vector<float>	px;
	std::vector<float>	py;
	
	std::vector<float>	rot;

	std::vector<float>	dx;
	std::vector<float>	dy;

	std::vector<bool>	alive;

	Particle();

	void create(float px, float py, float dx, float dy);
	void update(double dt);

	void draw(sf::RenderWindow& win);
};

