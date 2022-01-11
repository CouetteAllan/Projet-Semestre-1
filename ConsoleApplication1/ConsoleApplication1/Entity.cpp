#include "Entity.hpp"

Entity::Entity(Shape * shape, float _cx, float _cy, EType _type) {
	type = _type;
	cx = _cx;
	cy = _cy;
	sprite = shape;
	syncSprite();
}

Entity::Entity() {
	cx = 0;
	cy = 0;
	rx = 0.0f;
	ry = 0.0f;
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

void Entity::setState(State * currentState)
{
	if (currState)
		delete currState;

	currState = currentState;
	currState->onEnter();
}

Vector2f Entity::getPosition()
{
	return Vector2f(xx, yy);
}

void Entity::im()
{
	using namespace ImGui;

	Begin("Coordonates");
	static bool modified;
	modified = SliderInt("CX", &cx, 0.0f, Game::W / stride - 1);
	modified = SliderInt("CY", &cy, 0.0f, Game::H / stride);
	modified = SliderFloat("RX", &rx, 0.0f, 1.0f);
	modified = SliderFloat("RY", &ry, 0.0f, 1.0f);
	modified = Checkbox("Enable Gravity", &gravity);

	Value("Coord X", sprite->getPosition().x);
	Value("Coord Y", sprite->getPosition().y);
	Value("Speed X", dx);
	Value("Speed Y", dy);
	Value("IsGrounded", isGrounded);

	SliderFloat("Friction", &friction, 0.0f, 1.0f);
	if (modified)
		syncSprite();
	End();
}

void Entity::syncSprite()
{
	xx = floor((cx + rx) * stride);
	yy = floor((cy + ry) * stride);
	sprite->setPosition(xx, yy);
	if (circle)
		circle->setPosition(xx, yy);
}

void Entity::update( double dt)
{
	alive = HP > 0;
	if (alive) {

		rx += dx * dt;
		dx *= friction;//0.96f



		if (isColliding(cx + 2, cy) && rx >= 0.5f) {
			rx = 0.5f;
			dx = 0;
		}

		if (isColliding(cx - 1, cy) && rx <= 0.2f) {
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

		if ((isColliding(cx, cy - 1) && ry <= 0.5f) || isColliding(cx, cy + 1) && ry >= 0.5f) {
			ry = 0.5f;
			dy = 0;
		}

		while (ry > 1) {

			ry--;
			cy++;
		}
		while (ry < 0) {

			ry++;
			cy--;
		}

		collisionWithOtherEntities();

		syncSprite();
	}
	else {
		if (type == Enemy) {

		}
	}
}

inline bool Entity::overlaps(Entity * e) {
	auto maxDist = radius + e->radius;
	// classic distance formula
	auto distSqr = (e->xx - xx)*(e->xx - xx) + (e->yy - yy)*(e->yy - yy);
	return distSqr <= maxDist * maxDist;
}


void Entity::draw(RenderWindow &win)
{
	if (alive) {

		win.draw(*sprite);
		win.draw(*circle);
	}
}

void Entity::collisionWithOtherEntities()
{
	for (auto& o : World::objects) {
		if (o != this && o->alive) {
			// Fast distance check
			if (abs(cx - o->cx) <= 2 && abs(cy - o->cy) <= 2) {
				// Real distance check
				auto dist = sqrt((o->xx - xx)*(o->xx - xx) + (o->yy - yy)*(o->yy - yy));
				if (dist <= radius + o->radius) { //il y a overlapp
					auto ang = atan2(o->yy - yy, o->xx - xx);
					auto force = 700;
					auto repelPower = (radius + o->radius - dist) / (radius + o->radius);
					dx -= cos(ang) * repelPower * force;
					dy -= sin(ang) * repelPower * force;
					o->dx += cos(ang) * repelPower * force;
					o->dy += sin(ang) * repelPower * force;

					if (this->type == Player) {
						HP -= 1;
					}
				}
			}

			if (o->type == Bullet && this->type == Enemy) {
				BulletEntity* bu = (BulletEntity*)o;

				for (size_t i = 0; i < bu->pxx.size(); i++)
				{
					if (!bu->hit[i]) {
						if (abs(cx - bu->Arr_cx[i]) <= 2 && abs(cy - bu->Arr_cy[i]) <= 2) {
							// Real distance check
							auto dist = sqrt((bu->pxx[i] - xx)*(bu->pxx[i] - xx) + (bu->pyy[i] - yy)*(bu->pyy[i] - yy));
							if (dist <= radius + bu->radiusBullet[i]) { //il y a overlapp
								auto ang = atan2(bu->pyy[i] - yy, bu->pxx[i] - xx);
								auto force = 700;
								auto repelPower = (radius + bu->radiusBullet[i] - dist) / (radius + bu->radiusBullet[i]);
								dx -= cos(ang) * repelPower * force;
								dy -= sin(ang) * repelPower * force;

								if (this->type == Enemy) {
									static int id = 0;
									this->HP -= 1;
									if(HP != 0)
										this->sprite->setFillColor((sprite->getFillColor() + Color(127/HP,0,0)) - Color(0,127/HP,0,0));
									id++;
									printf("%i enemy touchey\n",id);
									bu->hit[i] = true;
									bu->alive[i] = false;
								}
							}
						}
					}
				}
			}

		}
	}
}

bool Entity::isColliding(int _cx, int _cy)
{
	if (_cx < 0)
		return true;
	if (_cy < 0)
		return true;
	if (_cx > 1280 / stride)
		return true;
	if (_cy > 720 / stride)
		return true;

	

	return false;

}

void PlayerEntity::update(double dt)
{
	if (currState) {
		currState->onUpdate(dt);
	}
	Entity::update(dt);
}



BulletEntity::BulletEntity()
{
	int t = 10;
	b = sf::CircleShape(t);
	b.setOutlineThickness(2);
	b.setFillColor(sf::Color::Yellow);
	b.setOutlineColor(sf::Color::Red);
	b.setOrigin(sf::Vector2f(t, t));
	type = Bullet;

}

void BulletEntity::create(float _px, float _py, float _dx, float _dy) {
	for (int i = 0; i < pxx.size(); ++i) {
		if (!alive[i]) {
			pxx[i] = _px;
			pyy[i] = _py;
			Arr_cx[i] = floor(_px / stride);
			Arr_cy[i] = floor(_py / stride);
			Arr_rx[i] = (_px - Arr_cx[i] * stride) / stride;
			Arr_ry[i] = (_py - Arr_cy[i] * stride) / stride;
			dx[i] = _dx;
			dy[i] = _dy;
			alive[i] = true;
			radiusBullet[i] = 10.0f;
			hit[i] = false;
			return;
		}
	}
	pxx.push_back(_px);
	pyy.push_back(_py);
	Arr_cx.push_back((int)(_px / stride));
	Arr_cy.push_back((int)(_py / stride));
	Arr_rx.push_back((_px - (int)(_px / stride) * stride) / stride);
	Arr_ry.push_back((_py - (int)(_py / stride) * stride) / stride);
	dx.push_back(_dx);
	dy.push_back(_dy);
	alive.push_back(true);
	radiusBullet.push_back(10.0f);
	hit.push_back(false);

}

void BulletEntity::update(double dt) {

	if (pxx.size() == 0)
		return;

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


void IdleState::onEnter()
{
	if (e->sprite) {
		delete e->sprite;
	}
	if (e->type == Player) {

		RectangleShape* playerShape = new RectangleShape(Vector2f(20, 45));
		playerShape->setFillColor(Color::Red);
		playerShape->setOrigin(playerShape->getSize().x / 2, playerShape->getSize().y / 2);
		e->sprite = playerShape;
	}
	else {
		RectangleShape* enemyShape = new RectangleShape(Vector2f(30, 60));
		enemyShape->setFillColor(Color::Red);
		enemyShape->setOrigin(enemyShape->getSize().x / 2, enemyShape->getSize().y / 2);
		e->sprite = enemyShape;
	}

}

void IdleState::onUpdate(double dt)
{

	bool movement = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ||
		abs(e->dx) > 0 && abs(e->dy) > 0;

	bool sprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

	if (movement)
		return e->setState(new WalkState(e));

	if (e->type == Player) {

		if (movement && sprint)
			return e->setState(new RunState(e));
	}
	else {

		auto dir = Game::player->getPosition() - e->getPosition();
		float dirLen = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		sf::Vector2f dxy(1, 0);
		if (dirLen) {
			dxy = dir / dirLen;
			return e->setState(new WalkState(e));
		}
	}
}

void WalkState::onEnter()
{
	if (e->sprite) {
		delete e->sprite;
	}
	if (e->type == Player) {

		RectangleShape* playerShape = new RectangleShape(Vector2f(20, 45));
		playerShape->setFillColor(Color::Green);
		playerShape->setOrigin(playerShape->getSize().x / 2, playerShape->getSize().y / 2);
		e->sprite = playerShape;
	}
	else {
		RectangleShape* playerShape = new RectangleShape(Vector2f(30, 60));
		playerShape->setFillColor(Color::Green);
		playerShape->setOrigin(playerShape->getSize().x / 2, playerShape->getSize().y / 2);
		e->sprite = playerShape;
	}
}

void WalkState::onUpdate(double dt)
{
	bool movement = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z);

	bool sprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	if (e->type == Player) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			e->dx += 300 * dt;

		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			e->dx -= 300 * dt;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			e->dy += 300 * dt;

		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			e->dy -= 300 * dt;
		}
	}
	else {
		auto dir = Game::player->getPosition() - e->getPosition();
		float dirLen = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		Vector2f dxy(1, 0);
		if (dirLen) {
			dxy = dir / dirLen;
		}
		dxy *= 3.0f;
		e->dx = dxy.x;
		e->dy = dxy.y;
	}


	if (abs(e->dx) <= 1 && abs(e->dy) <= 1) {
		return e->setState(new IdleState(e));
	}

	if (e->type == Player) {

		if (sprint && movement)
			return e->setState(new RunState(e));

	}

}

void RunState::onEnter()
{
	if (e->sprite) {
		delete e->sprite;
	}
	RectangleShape* playerShape = new RectangleShape(Vector2f(20, 45));
	playerShape->setFillColor(Color::Blue);
	playerShape->setOrigin(playerShape->getSize().x / 2, playerShape->getSize().y / 2);
	e->sprite = playerShape;

}

void RunState::onUpdate(double dt)
{
	bool movement = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z);

	bool sprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sprint) {
		e->dx += 2 * 2;

	} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && sprint) {
		e->dx -= 2 * 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sprint) {
		e->dy += 2 * 2;

	} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sprint) {
		e->dy -= 2 * 2;
	}

	if (abs(e->dx) <= 1 && abs(e->dy) <= 1) {
		e->setState(new IdleState(e));
	}

	if (!sprint && movement)
		e->setState(new WalkState(e));
}

void EnemyEntity::update(double dt)
{
	if (currState) {
		currState->onUpdate(dt);
	}
	Entity::update(dt);
}
