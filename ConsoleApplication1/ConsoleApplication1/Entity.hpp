#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include "Game.hpp"


using namespace sf;
class Entity;

class State {
public:
	Entity*		e = nullptr;

	virtual void onEnter() = 0;
	virtual void onUpdate(double dt) = 0;

};


class IdleState : public State {
public:
	float timer = 1.0f;
	float timerReset = timer;

	IdleState(Entity* _e) {
		e = _e;
	}

	virtual void onEnter();
	virtual void onUpdate(double dt);
};

class WalkState : public State {
public:
	WalkState(Entity* _e) {
		e = _e;
	}

	virtual void onEnter();
	virtual void onUpdate(double dt);

};

class RunState : public State {
public:
	RunState(Entity* _e) {
		e = _e;
	}

	virtual void onEnter();
	virtual void onUpdate(double dt);

};

enum EType {
	Player,
	Enemy,
	Bullet,
	Dead,
};


class Entity {
public:


	//---------- Graphic Components ------------//
	Shape*				sprite = nullptr;
	Sprite*				spr = nullptr;
	static Texture				text;
	bool				visible = true;
	float				radius = 0.0f; //hitbox
	CircleShape*		circle = nullptr;
	EType				type;

	//---------- Base Coordinates -------------------//
	int				cx = 0; //Numéro de la cellule en X
	int				cy = 0; //Numéro de la cellule en Y
	float			rx = 0.0f; //Ratio de la cellule en X allant de 0.0f à 1.0f
	float			ry = 0.0f; //Ratio de la cellule en Y allant de 0.0f à 1.0f

	//-------------- Movement -------------------------//
	float				dx = 0.0f;
	float				dy = 0.0f;
	static float friction;
	float				speedMultiplier = 1;


	//-------------- Resulting Coordinates -----------------//
	double			xx = 0.0f;
	double			yy = 0.0f;

	static int stride;

	int				HPMax = 2;
	int				HP;
	bool			alive = true;

	Vector2f	playerOrientation;
	void setMousePos(Vector2f mousePos);

	int click = 0;

	Entity(Shape* shape, float _cx, float _cy, EType _type);
	Entity(float _cx, float _cy, EType _type);
	Entity();

	State*			currState;
	void setPosition(float x, float y);
	void setState(State* currentState);

	Vector2f getPosition();
	void im();
	void syncSprite();
	void update( double dt);
	bool overlaps(Entity* e);
	void draw(RenderWindow &win);
	void collisionWithOtherEntities();

	bool isColliding(int _cx, int _cy);
protected:
	float timer = 0;
	float timerReset = 2;
};


class PlayerEntity : public Entity {
public:
	Clock clock;
	IntRect		textureRect;

	virtual void update(double dt);
	void		 heal(int& score);
	
	PlayerEntity(Shape* shape, float _cx, float _cy, EType _type = Player) : Entity(shape, _cx, _cy, _type) {
		radius = 18.0f;
		circle = new CircleShape(radius);
		circle->setFillColor(Color(255, 0, 0, 100));
		circle->setOrigin(radius,radius);
		setState(new IdleState(this));

		if (Entity::text.loadFromFile("res/spriteSheetHero.png")) {
			spr = new Sprite(Entity::text);
			textureRect = IntRect(0, 0, 128, 128);
			spr->setTextureRect(textureRect);
			spr->setOrigin(64, 54);
			spr->setScale(0.5f, 0.5f);
		}
		syncSprite();
		HPMax = 5;
		HP = HPMax;
		
	}
};

class BulletEntity : public Entity {
public:
	CircleShape b;

	std::vector<int>	Arr_cx; //Tableau Numéro de la cellule en X
	std::vector<int>	Arr_cy; //Tableau Numéro de la cellule en Y
	std::vector<float>	Arr_rx; //Tableau Ratio de la cellule en X allant de 0.0f à 1.0f
	std::vector<float>	Arr_ry; //Tableau Ratio de la cellule en Y allant de 0.0f à 1.0f

	std::vector<float>	radiusBullet;

	std::vector<bool>	hit;

	std::vector<float>	pxx;
	std::vector<float>	pyy;

	std::vector<float>	dx;
	std::vector<float>	dy;

	std::vector<bool>	alive;


	BulletEntity();

	void create(float pxx, float pyy, float dx, float dy);
	virtual void update(double dt);
	virtual void draw(RenderWindow &win);
	void convert(int i);

};

class EnemyEntity : public Entity {
public:

	EnemyEntity(Shape* shape, float _cx, float _cy, EType _type = Enemy) : Entity(shape,_cx,_cy,_type) {
		setState(new IdleState(this));
		circle = new CircleShape(radius);
		circle->setFillColor(Color(255, 0, 0, 100));
		circle->setOrigin(radius, radius);
	}

	int enemySize = 1;

	~EnemyEntity() {
		if (sprite)
			delete sprite;

		if (circle)
			delete circle;


	}

	virtual void update(double dt);

};
