#include "World.hpp"
#include "Entity.hpp"

std::vector<Entity*> World::objects;
std::vector<RectangleShape*> World::grid;

void World::draw(RenderWindow& win)
{
	for (auto o : objects) {
		o->draw(win);
	}
	for (auto r : grid) {
		win.draw(*r);
	}
}

void World::update(double dt)
{
	Entity* p = nullptr;
	for (auto o : objects) {
		o->update(dt);

	}
}
