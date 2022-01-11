#include "Particles.hpp"

Particle::Particle() {
	if (particleText.loadFromFile("res/particle.png"))
		part.setTexture(particleText);
	part.setScale(Vector2f(0.3f,0.3f));
}

void Particle::create(float _px, float _py, float _dx, float _dy) {
	for (int i = 0; i < px.size(); ++i) {
		if (!alive[i]) {
			px[i] = _px;
			py[i] = _py;
			dx[i] = _dx;
			dy[i] = _dy;
			alive[i] = true;
			return;
		}
	}
	px.push_back(_px);
	py.push_back(_py);
	dx.push_back(_dx);
	dy.push_back(_dy);
	alive.push_back(true);
}

void Particle::update(double dt) {
	for (int i = 0; i < px.size(); ++i) {
		if (alive[i]) {
			px[i] += dx[i] * 3 * dt;
			py[i] += dy[i] * 3 * dt;

			//[i] += (9.0f * 50.0f) * dt;

			if (
				(px[i] > 3000) || (px[i] < -100) ||
				(py[i] > 3000) || (py[i] < -100)
				) {
				alive[i] = false;
			}
		}
	}
}

void Particle::draw(sf::RenderWindow& win) {
	int idx = 0;
	const int sz = px.size();
	while (idx < sz) {
		if (alive[idx]) {
			part.setPosition(px[idx], py[idx]);
			part.rotate(1);
			win.draw(part);
		}
		idx++;
	}
}
