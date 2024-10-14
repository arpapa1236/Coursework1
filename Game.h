#pragma once
#include "STD.h"
#include "Player.h"
#include "Textur.h"
#include "Sprite.h"
#include "Global.h"
#include "Enemies.h"
#include "Bullets.h"
#include "Boosts.h"
#include "Text.h"
void drawPlayer(Player* player);
void drawEnemy(Enemy* enemy, int type);
void drawBoost(Boost* boost, int type);
int Game();