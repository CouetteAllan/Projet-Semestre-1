#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <math.h>
#include "World.hpp"


using namespace sf;

enum EType {
	Player,
	Wall,
	Enemy,
};

class Entity {
public:


	//---------- Graphic Components ------------//
	Shape* sprite = nullptr;
	bool visible = true;
	EType type;

	//---------- Base Coordinates -------------------//
	int cx = 0; //Numéro de la cellule en X
	int cy = 0; //Numéro de la cellule en Y
	float rx = 0.0f; //Ratio de la cellule en X allant de 0.0f à 1.0f
	float ry = 0.0f; //Ratio de la cellule en Y allant de 0.0f à 1.0f

	//-------------- Movement -------------------------//
	float dx = 0.0f;
	float dy = 0.0f;
	inline static float friction = 0.827f;
	bool isGrounded = false;
	bool gravity = true;


	//-------------- Resulting Coordinates -----------------//
	double xx = 0.0f;
	double yy = 0.0f;

	const inline static int stride = 32;


	int click = 0;

	Entity( Shape* shape, float _cx, float _cy, EType _type = Player) {
		type = _type;
		cx = _cx;
		cy = _cy;
		sprite = shape;
		syncSprite();
	}


	void setPosition(float x, float y);

	void syncSprite();
	void update(double dt);
	void handleCollisions(Entity* e = nullptr);
	void draw(RenderWindow &win);

	bool isColliding(int _cx, int _cy);
};

