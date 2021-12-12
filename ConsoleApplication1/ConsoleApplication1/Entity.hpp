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
	float			dx = 0.0;
	float			dy = 0.0f;
};