#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "World.hpp"
#include "Particles.hpp"
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
class Particle;	
class Game
{
public:
	static inline const int W = 1280;
	static inline const int H = 720;

	static int score;

	static int			shake;
	static Particle		parts;

	static PlayerEntity*  player;

	static std::vector<sf::Vector2i> walls;

	static void			particlesAt(sf::Vector2f pos);
	static void			init();
	static void			im();


};

class sf::Sound;
class sf::SoundBuffer;
class sf::Music;

class Audio {
public:
	Audio();
	static	sf::Sound			laserShoot;
	static sf::SoundBuffer		laserShootBuffer;
		
	static	sf::Music			bgm;
	
	static	sf::Sound			explosion;
	static	sf::SoundBuffer		explosionBuffer;
		
	static	sf::Sound			smallLaser;
	static	sf::SoundBuffer		smallLaserBuffer;
};

