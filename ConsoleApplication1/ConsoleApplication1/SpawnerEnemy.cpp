#include "SpawnerEnemy.hpp"

int SpawnerEnemy::numberOfEnemies = 0;
int SpawnerEnemy::numberOfEnemiesMax = 8;
int SpawnerEnemy::wavesEnemy = 1;
SpawnerEnemy::SpawnerEnemy()
{
}

void SpawnerEnemy::spawnAtLocation(int _cx, int _cy)
{
	if (numberOfEnemies > numberOfEnemiesMax) {
		return;
	}

	int roll = rand() % 99 + 1;
	sizeIdx = roll;
	if (sizeIdx <= 33) {
		sizeIdx = 1;
		sizeX = 20.0f;
		sizeY = 50.0f;
	}
	else if (sizeIdx > 66) {
		sizeIdx = 3;
		sizeX = 45.0f;
		sizeY = 100.0f;
	}
	else {
		sizeIdx = 2;
		sizeX = 35.0f;
		sizeY = 70.0f;
	}


	RectangleShape* enemyShape = new RectangleShape(Vector2f(sizeX,sizeY));
	enemyShape->setFillColor(Color::Green);
	enemyShape->setOrigin(enemyShape->getSize().x / 2, enemyShape->getSize().y / 2);

	EnemyEntity* enemy = new EnemyEntity(enemyShape,_cx,_cy);
	numberOfEnemies++;
	switch (sizeIdx) {
	case 1:
		enemy->HPMax = 5;
		enemy->radius = 15.0f;
		enemy->speedMultiplier = 1.1f;
		if (wavesEnemy == 6) {
			enemy->speedMultiplier = 1.3f;
		}
		break;

	case 2:
		enemy->HPMax = 7;
		enemy->radius = 25.0f;
		enemy->speedMultiplier = 1.0f;
		if (wavesEnemy == 6) {
			enemy->speedMultiplier = 1.2f;
		}

		break;

	case 3:
		enemy->HPMax = 10;
		enemy->radius = 35.0f;
		enemy->speedMultiplier = 0.8f;
		if (wavesEnemy == 6) {
			enemy->speedMultiplier = 1.0f;
		}

		break;

	default:
		break;
	}
	enemy->HP = enemy->HPMax;
	enemy->enemySize = sizeIdx;
	World::objects.push_back(enemy);
}
