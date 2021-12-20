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
	Bullet,
};

class Entity {
public:


	//---------- Graphic Components ------------//
	Shape*			sprite = nullptr;
	bool			visible = true;
	EType			type;

	//---------- Base Coordinates -------------------//
	int				cx = 0; //Numéro de la cellule en X
	int				cy = 0; //Numéro de la cellule en Y
	float			rx = 0.0f; //Ratio de la cellule en X allant de 0.0f à 1.0f
	float			ry = 0.0f; //Ratio de la cellule en Y allant de 0.0f à 1.0f

	//-------------- Movement -------------------------//
	float				dx = 0.0f;
	float				dy = 0.0f;
	inline static float friction = 0.827f;
	bool				isGrounded = false;
	bool				gravity = true;


	//-------------- Resulting Coordinates -----------------//
	double			xx = 0.0f;
	double			yy = 0.0f;

	const inline static int stride = 32;


	int click = 0;

	Entity(Shape* shape, float _cx, float _cy, EType _type);
	Entity();


	void setPosition(float x, float y);

	Vector2f getPosition();

	void syncSprite();
	void update(double dt);
	void handleCollisions(Entity* e = nullptr);
	void draw(RenderWindow &win);

	bool isColliding(int _cx, int _cy);
};


class PlayerEntity : public Entity {
public:

	int				HP = 5;
	float			speedMultiplier = 1.2f;

	virtual void update(double dt);
	
	PlayerEntity(Shape* shape, float _cx, float _cy, EType _type = Player) : Entity(shape, _cx, _cy, _type) {

	}
};

class BulletEntity : public Entity {
public:
	CircleShape b;

	std::vector<int>	Arr_cx; //Tableau Numéro de la cellule en X
	std::vector<int>	Arr_cy; //Tableau Numéro de la cellule en Y
	std::vector<float>	Arr_rx; //Tableau Ratio de la cellule en X allant de 0.0f à 1.0f
	std::vector<float>	Arr_ry; //Tableau Ratio de la cellule en Y allant de 0.0f à 1.0f

	std::vector<float>	pxx;
	std::vector<float>	pyy;

	std::vector<float>	dx;
	std::vector<float>	dy;

	std::vector<bool>	alive;

	BulletEntity() : Entity()
	{
		int t = 10;
		b = sf::CircleShape(t);
		b.setOutlineThickness(2);
		b.setFillColor(sf::Color::Yellow);
		b.setOutlineColor(sf::Color::Red);
		b.setOrigin(sf::Vector2f(t, t));
		sprite = (Shape*)&b;

	}

	void create(float pxx, float pyy, float dx, float dy);
	virtual void update(double dt);
	virtual void draw(RenderWindow &win);
	void convert(int i);
	void draw(sf::RenderWindow& win);

};
