#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

using namespace sf;

class Entity {
public:
	//---------------- Graphic Components  -----------------------
	Shape*			spr = nullptr;
	bool			isVisible = true;

	//---------------- Coordinates -------------------------------
	int				cx = 0;
	int				cy = 0;
	float			rx = 0.0f;
	float			ry = 0.0f;

	//---------------  Movements   -------------------------------
	float			dx = 0.0;
	float			dy = 0.0f;

	Entity(Shape* shape, int x, int y) {
		spr = shape;
		cx = x;
		cy = y;
	}

	void update(double dt);
	void draw(RenderWindow& win);

protected:
	void syncSprite();

};