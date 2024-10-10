#pragma once
void fireBasic(void* owner, Bullet* bullets, int maxBullets, Enemy* enemies, int numEnemies);
void fireTripleShot(void* owner, Bullet* bullets, int maxBullets, Enemy* enemies, int numEnemies);
void fireEnemyBasic(void* owner, Bullet* bullets, int maxBullets, Player* player, int numEnemies);
void initWeapon(Weapon* weapon, int type);
void drawBullet(SDL_Renderer* renderer, Bullet* bullet);
bool checkBulletCollision(Bullet bullet, Enemy enemy);
bool checkEnemyBulletCollision(Bullet bullet, Player player);
bool AreaDamage(Player* player, Enemy* enemy, double radiusAreaDamage);
bool AreaDamageforEnemy(Player* player, Enemy* enemy, double radiusAreaDamage);