#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "World.hpp"
#include "HUD.hpp"
#include "Particles.hpp"
#include "Entity.hpp"
#include "SpawnerEnemy.hpp"
#include <algorithm>
#include "imgui.h"
#include "imgui-SFML.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <math.h>


class PlayerEntity;
class Particle;	
class Game
{
public:
	static const int W = 1280;
	static const int H = 720;

	static int score;
	static int wave;

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
	sf::SoundBuffer				laserShootBuffer;
		
	static	sf::Music			bgm;
	
	static	sf::Sound			explosion;
	sf::SoundBuffer				explosionBuffer;
		
	static	sf::Sound			gameOver;
	sf::SoundBuffer				gameOverBuffer;
	
	static	sf::Sound			hit;
	sf::SoundBuffer				hitBuffer;
	
	static	sf::Sound			heal;
	sf::SoundBuffer				healBuffer;
	
	static	sf::Sound			wave;
	sf::SoundBuffer				waveBuffer;
};

