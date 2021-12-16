#include "Entity.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "imgui.h"
#include "World.hpp"

void Entity::setPosition(float x, float y)
{
	xx = x;
	yy = y;
	cx = floor(xx / stride);
	cy = floor(yy / stride);
	rx = (xx - cx * stride) / stride;
	ry = (yy - cy * stride) / stride;


}

void Entity::syncSprite()
{
	xx = floor((cx + rx) * stride);
	yy = floor((cy + ry) * stride);
	sprite->setPosition(xx, yy);
}

void Entity::update(double dt)
{
	if (type == Wall) {
		return;
	}
	rx += dx * dt;
	dx *= friction;//0.96f


	
	if (isColliding(cx + 1, cy)&& rx >= 0.8f) {
		rx = 0.8f;
		dx = 0;
	}
	
	if (isColliding(cx - 1, cy)&& rx <= 0.2f) {
		rx = 0.2f;
		dx = 0;
	}

	while (rx > 1) {

		rx--;
		cx++;
	}
	while (rx < 0) {

		rx++; 
		cx--;
	}


	ry += dy * dt;
	if(!isGrounded && type != Wall && gravity)
		dy += 80.0f * dt;
	dy *= 0.95f;
	
	if (isColliding(cx, cy - 1) && ry <= 0.0f) {
		ry = 1.0f;
		dy = 0;
	}
	
	if (isColliding(cx, cy + 1) && ry >= 0.9f) {
		ry = 1.0f;
		dy = 0;
		isGrounded = true;
	}
	else
		isGrounded = false;

	while (ry > 1) {

		ry--;
		cy++;
	}
	while (ry < 0) {

		ry++;
		cy--;
	}

	syncSprite();
}


void Entity::draw(RenderWindow &win)
{
	win.draw(*sprite);
}

bool Entity::isColliding(int _cx, int _cy)
{
	if (cx < 0)
		return true;
	if (cy < 0)
		return true;
	if (cx > 1280 / stride)
		return true;
	if (cy > 720 / stride)
		return true;
	
	for (auto& w : World::objects) {
		if (w->type != Player) {

			if (w->cx == _cx && w->cy == _cy)
				return true;
		}
	}

	return false;

	//regarde si il y a quelque chose en (_cx,_cy);
	//cx 
}
