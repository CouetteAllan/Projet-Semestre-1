#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include "World.hpp"
#include "Entity.hpp"
#include <algorithm>
#include "imgui.h"
#include "imgui-SFML.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <math.h>


class PlayerEntity;

class Game
{
public:
	static inline const int W = 1280;
	static inline const int H = 720;


	static PlayerEntity*  player;

	static std::vector<sf::Vector2i> walls;
	static void			init();
	static void			im();


};


class Audio {

};

