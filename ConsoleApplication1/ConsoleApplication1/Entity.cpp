#include "Entity.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "imgui.h"
#include "World.hpp"

inline Entity::Entity(Shape * shape, float _cx, float _cy, EType _type) {
	type = _type;
	cx = _cx;
	cy = _cy;
	sprite = shape;
	syncSprite();
}

inline Entity::Entity() {
	cx = 0;
	cy = 0;
	rx = 0.0f;
	ry = 0.0f;
	syncSprite();
}

void Entity::setPosition(float x, float y)
{
	xx = x;
	yy = y;
	cx = floor(xx / stride);
	cy = floor(yy / stride);
	rx = (xx - cx * stride) / stride;
	ry = (yy - cy * stride) / stride;


}

Vector2f Entity::getPosition()
{
	return Vector2f(xx,yy);
}

void Entity::syncSprite()
{
	xx = floor((cx + rx) * stride);
	yy = floor((cy + ry) * stride);
	sprite->setPosition(xx, yy);
}

void Entity::update(double dt)
{
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
	dy *= friction;
	
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
		if (w->type != Player || w->type != Enemy) {

			if (w->cx == _cx && w->cy == _cy)
				return true;
		}
	}

	return false;
	
}

void PlayerEntity::update(double dt)
{
	Entity::update(dt);
}

void BulletEntity::create(float _px, float _py, float _dx, float _dy) {
	for (int i = 0; i < pxx.size(); ++i) {
		if (!alive[i]) {
			pxx[i] = _px;
			pyy[i] = _py;
			dx[i] = _dx;
			dy[i] = _dy;
			alive[i] = true;
			return;
		}
	}
	pxx.push_back(_px);
	pyy.push_back(_py);
	dx.push_back(_dx);
	dy.push_back(_dy);
	alive.push_back(true);
}

void BulletEntity::update(double dt) {
	for (int i = 0; i < pxx.size(); ++i) {
		if (alive[i]) {
			Arr_rx[i] += dx[i] * dt;

			while (Arr_rx[i] > 1) {
				Arr_rx[i]--;
				Arr_cx[i]++;
			}
			while (Arr_rx[i] < 0) {
				Arr_rx[i]++;
				Arr_cx[i]--;
			}

			Arr_ry[i] += dy[i] * dt;

			while (Arr_ry[i] > 1) {
				Arr_ry[i]--;
				Arr_cy[i]++;
			}
			while (Arr_ry[i] < 0) {
				Arr_ry[i]++;
				Arr_cy[i]--;
			}
			convert(i);
			if (
				(pxx[i] > 2000) || (pxx[i] < -10) ||
				(pyy[i] > 1000) || (pyy[i] < -10)
				) 
			{
				alive[i] = false;
			}
		}
	}
}

void BulletEntity::draw(sf::RenderWindow& win) {
	int idx = 0;
	const int sz = pxx.size();
	while (idx < sz) {
		if (alive[idx]) {
			b.setPosition(pxx[idx], pyy[idx]);
			win.draw(b);
		}
		idx++;
	}
}

void BulletEntity::convert(int i)
{
	pxx[i] = (Arr_cx[i] + Arr_rx[i]) * stride;
	pyy[i] = (Arr_cy[i] + Arr_ry[i]) * stride;
}

