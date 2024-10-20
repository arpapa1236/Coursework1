#pragma once
#include "Global.h"
#include "STD.h"
#include "Game.h"
#include "Bullets.h"
#include "Boosts.h"
#include "Sprite.h"
void checkCollisions(Enemy* enemies, int numEnemies, int current);
void updateRunningEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current);
void updateShootingEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current);
void initEnemy(Enemy* e, int type, double x, double y, int numOfWave);
void spawnEnemies(Enemy* enemies, int numEnemies, int numOfWave);
void updatestaticShootingEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current);