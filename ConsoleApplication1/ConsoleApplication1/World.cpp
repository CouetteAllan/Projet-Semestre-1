#include "World.hpp"
#include "Entity.hpp"

std::vector<Entity*> World::objects;
std::vector<RectangleShape*> World::grid;

void World::draw(RenderWindow& win)
{
	for (auto o : objects) {
		if (o->type == Bullet) {
			BulletEntity* bu = (BulletEntity*)o;
			bu->draw(win);
		}
		else
			o->draw(win);
	}
	for (auto r : grid) {
		win.draw(*r);
	}

	Game::parts.draw(win);
}

void World::update(double dt)
{
	Entity* p = nullptr;
	for (auto o : objects) {
		if (o->type == Bullet) {
			BulletEntity* bu = (BulletEntity*)o;
			bu->update(dt);
		}
		else if (o->type == Player) {
			PlayerEntity* p = (PlayerEntity*)o;
			p->update(dt);
		}
		else if (o->type == Enemy) {
			EnemyEntity* en = (EnemyEntity*)o;
			en->update(dt);
		}
		else
			o->update(dt);

	}

	Game::parts.update(dt);
}
