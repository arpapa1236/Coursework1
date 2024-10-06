#include "Global.h"
#include "STD.h"
#include "Game.h"
#include "Enemies.h"
#include "Bullets.h"
#include "Boosts.h"
void fireBasic(void* owner, Bullet* bullets, int maxBullets, Enemy* enemies, int numEnemies) {
    Player* p = (Player*)owner;
    Enemy* closestEnemy = NULL;
    double minDistance = DBL_MAX;  // Используйте значение максимально возможного double для начального сравнения

    // Поиск ближайшего активного врага
    for (int j = 0; j < numEnemies; j++) {
        if (enemies[j].active) {
            double dx = enemies[j].x - p->x;
            double dy = enemies[j].y - p->y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < minDistance) {
                minDistance = distance;
                closestEnemy = &enemies[j];
            }
        }
    }

    // Если ближайший враг найден, стреляем
    if (closestEnemy != NULL) {
        for (int i = 0; i < maxBullets; i++) {
            if (!bullets[i].active) {
                bullets[i].active = true;
                bullets[i].x = p->x;
                bullets[i].y = p->y;
                // Вычисление нормированного направления к ближайшему врагу
                double dx = closestEnemy->x - p->x;
                double dy = closestEnemy->y - p->y;
                double length = sqrt(dx * dx + dy * dy);
                bullets[i].dx = dx / length * BULLET_SPEED;  // Умножаем на скорость пули
                bullets[i].dy = dy / length * BULLET_SPEED;
                bullets[i].owner = OWNER_PLAYER;
                break;  // Прерываем цикл после активации первой неактивной пули
            }
        }
    }
}
void fireEnemyBasic(void* owner, Bullet* bullets, int maxBullets, Player* player, int numEnemies) {
    Enemy* e = (Enemy*)owner;

    for (int i = 0; i < maxBullets; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].x = e->x;
            bullets[i].y = e->y;
            double dx = player->x - e->x;
            double dy = player->y - e->y;
            double length = sqrt(dx * dx + dy * dy);
            bullets[i].dx = dx / length * BULLET_SPEED;
            bullets[i].dy = dy / length * BULLET_SPEED;
            bullets[i].owner = OWNER_ENEMY;
            break;  // Прерываем цикл после активации первой неактивной пули
        }
    }
}
void fireTripleShot(void* owner, Bullet* bullets, int maxBullets, Enemy* enemies, int numEnemies) {
    Player* p = (Player*)owner;
    int bulletsFired = 0;
    // Определяем базовое направление стрельбы на основе нажатых клавиш
    double baseDx = 0.0;
    double baseDy = 0.0;
    if (keyboard[SDL_SCANCODE_UP]) {
        baseDy -= 1;
    }
    if (keyboard[SDL_SCANCODE_DOWN]) {
        baseDy += 1;
    }
    if (keyboard[SDL_SCANCODE_LEFT]) {
        baseDx -= 1;
    }
    if (keyboard[SDL_SCANCODE_RIGHT]) {
        baseDx += 1;
    }
    double baseLength = sqrt(baseDx * baseDx + baseDy * baseDy);
    if (baseLength == 0) { // Если нет нажатий, стреляем по умолчанию вправо
        baseDx = 1.0;
    }
    else {
        baseDx /= baseLength;
        baseDy /= baseLength;
    }
    // Определение смещения в зависимости от направления стрельбы
    double offset = 0.2;
    double directions[3][2];
    if (fabs(baseDx) > fabs(baseDy)) {
        // Горизонтальное направление стрельбы
        directions[0][0] = baseDx; directions[0][1] = baseDy - offset;
        directions[1][0] = baseDx; directions[1][1] = baseDy;
        directions[2][0] = baseDx; directions[2][1] = baseDy + offset;
    }
    else {
        // Вертикальное направление стрельбы
        directions[0][0] = baseDx - offset; directions[0][1] = baseDy;
        directions[1][0] = baseDx; directions[1][1] = baseDy;
        directions[2][0] = baseDx + offset; directions[2][1] = baseDy;
    }

    for (int i = 0; i < maxBullets && bulletsFired < 3; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].x = p->x + PLAYER_WIDTH / 2;
            bullets[i].y = p->y + PLAYER_HEIGHT / 2;

            // Выбор направления из предустановленных
            double dx = directions[bulletsFired][0];
            double dy = directions[bulletsFired][1];
            double length = sqrt(dx * dx + dy * dy);

            bullets[i].dx = dx / length * BULLET_SPEED;
            bullets[i].dy = dy / length * BULLET_SPEED;
            bullets[i].owner = OWNER_PLAYER;
            bulletsFired++;
        }
    }
}
void fireAutoAim(void* owner, Bullet* bullets, int maxBullets, Enemy* enemies, int numEnemies) {
    Player* p = (Player*)owner;
    Enemy* closestEnemy = NULL;
    double minDistance = DBL_MAX;

    // Поиск ближайшего активного врага
    for (int j = 0; j < numEnemies; j++) {
        if (enemies[j].active) {
            double dx = enemies[j].x - p->x;
            double dy = enemies[j].y - p->y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < minDistance) {
                minDistance = distance;
                closestEnemy = &enemies[j];
            }
        }
    }

    // Если ближайший враг найден, стреляем
    if (closestEnemy != NULL) {
        for (int i = 0; i < maxBullets; i++) {
            if (!bullets[i].active) {
                bullets[i].x = p->x;
                bullets[i].y = p->y;
                bullets[i].target = closestEnemy;
                bullets[i].active = true;
                double dx = closestEnemy->x - bullets[i].x;
                double dy = closestEnemy->y - bullets[i].y;
                double length = sqrt(dx * dx + dy * dy);
                bullets[i].dx = dx / length * BULLET_SPEED * 0.5;
                bullets[i].dy = dy / length * BULLET_SPEED;
                break;
            }
        }
    }
}
bool AreaDamage(Player* player, Enemy* enemy, double radiusAreaDamage)
{
    double dx = player->x - enemy->x;
    double dy = player->y - enemy->y;
    double distance = sqrt(dx * dx + dy * dy);
    return distance <= radiusAreaDamage;
}
void initWeapon(Weapon* weapon, int type) {
    weapon->type = type;
    weapon->fireRate = 1.0; // Пример: выстрелы 1 раз в секунду
    weapon->nextFireTime = SDL_GetTicks() + (1000 / weapon->fireRate); // Начальное время для первого выстрела
    switch (type) {
    case WEAPON_TYPE_BASIC:
        weapon->fire = fireBasic;
        break;
    case WEAPON_TYPE_AUTOAIM:
        weapon->fire = fireAutoAim;
        break;
    case WEAPON_TYPE_TRIPLE_SHOT:
        weapon->fire = fireTripleShot;
        break;
    case WEAPON_TYPE_ENEMY:
        weapon->enfire = fireEnemyBasic;
        break;
        // Другие типы оружия
    }
}
void drawBullet(SDL_Renderer* renderer, Bullet* bullet) {
    if (bullet->active) {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_Rect bulletRect = { bullet->x, bullet->y, BULLET_SIZE, BULLET_SIZE };
        SDL_RenderFillRect(renderer, &bulletRect);
    }
}
bool checkBulletCollision(Bullet bullet, Enemy enemy) {
    return bullet.x < enemy.x + ENEMY_SIZE &&
        bullet.x + BULLET_SIZE > enemy.x &&
        bullet.y < enemy.y + ENEMY_SIZE &&
        bullet.y + BULLET_SIZE > enemy.y;
}
bool checkEnemyBulletCollision(Bullet bullet, Player player) {
    return bullet.x < player.x + PLAYER_WIDTH &&
        bullet.x + BULLET_SIZE > player.x &&
        bullet.y < player.y + PLAYER_WIDTH &&
        bullet.y + BULLET_SIZE > player.y;
}