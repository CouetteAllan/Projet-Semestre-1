#pragma once

#include "Game.hpp"


class SpawnerEnemy
{

public:
	
	int cx = 0;
	int cy = 0;

	int sizeIdx = 1;

	float sizeX = 30.0f;
	float sizeY = 60.0f;

	SpawnerEnemy();
	
	
	void spawnAtLocation(int _cx,int _cy);

};

