#pragma once
#include "Entity.hpp"
#include <iostream>
#include "Game.hpp"

class Entity;

class World {
public:
	static std::vector<sf::RectangleShape* > grid;
	static std::vector<Entity*> objects;


	static void draw(sf::RenderWindow& win);
	static void update(double dt);


};