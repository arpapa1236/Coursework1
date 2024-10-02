#include "Enemies.h"
void checkCollisions(Enemy* enemies, int numEnemies, int current)//  оллизи€ врагов между собой
// ѕо диагонали проблемно потом доработать об€зательно что тут что на игроке, на пуле мб тоже
{
    for (int i = 0; i < numEnemies; i++) {
        if (i != current) {
            double dx = enemies[i].x - enemies[current].x;
            double dy = enemies[i].y - enemies[current].y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < ENEMY_SIZE) {
                enemies[current].x -= dx / distance * (ENEMY_SIZE - distance) / 2;
                enemies[current].y -= dy / distance * (ENEMY_SIZE - distance) / 2;
                enemies[i].x += dx / distance * (ENEMY_SIZE - distance) / 2;
                enemies[i].y += dy / distance * (ENEMY_SIZE - distance) / 2;
            }
        }
    }
}
void updateRunningEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current) //раннер
//¬родь норм
{
    Enemy* e = (Enemy*)enemy;
    Player* p = (Player*)player;
    Enemy* es = (Enemy*)enemies;
    double dx = p->x - e->x;
    double dy = p->y - e->y;
    double length = sqrt(dx * dx + dy * dy);
    if (length > PLAYER_WIDTH) {
        dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
        dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
        e->x += dx;
        e->y += dy;
    }
    else {
        e->x = p->x - ((dx / length) * PLAYER_WIDTH);
        e->y = p->y - ((dy / length) * PLAYER_HEIGHT);
    }
    if (e->x < 0) e->x = 0;
    else if (e->x > WIN_WIDTH - PLAYER_WIDTH) e->x = WIN_WIDTH - PLAYER_WIDTH;

    if (e->y < 0) e->y = 0;
    else if (e->y > WIN_HEIGHT - PLAYER_HEIGHT) e->y = WIN_HEIGHT - PLAYER_HEIGHT;
    checkCollisions(es, numEnemies, current);
}
void updateShootingEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current) {
    Enemy* e = (Enemy*)enemy;
    Player* p = (Player*)player;
    Enemy* es = (Enemy*)enemies;
    double dx = p->x - e->x;
    double dy = p->y - e->y;
    double length = sqrt(dx * dx + dy * dy);
    double range = 150; // ƒопустимый радиус, в пределах которого враг не будет приближатьс€ к игроку
    static double lastShootTime = 0;
    double currentTime = SDL_GetTicks() / 1000.0;

    // ќпределение, должен ли враг двигатьс€ или остановитьс€
    if (length > range) {
        dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
        dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
        e->x += dx;
        e->y += dy;
    }
    else {
        // ¬раг останавливаетс€ на границе радиуса
        e->x = p->x - (dx / length) * range;
        e->y = p->y - (dy / length) * range;
    }
    if (e->x < 0) e->x = 0;
    else if (e->x > WIN_WIDTH - PLAYER_WIDTH) e->x = WIN_WIDTH - PLAYER_WIDTH;

    if (e->y < 0) e->y = 0;
    else if (e->y > WIN_HEIGHT - PLAYER_HEIGHT) e->y = WIN_HEIGHT - PLAYER_HEIGHT;

    checkCollisions(es, numEnemies, current);
}
void updatestaticShootingEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current) {
    Enemy* e = (Enemy*)enemy;
    Player* p = (Player*)player;
    Enemy* es = (Enemy*)enemies;
    double dx = p->x - e->x;
    double dy = p->y - e->y;
    double length = sqrt(dx * dx + dy * dy);
    double range = 150;
    static double lastShootTime = 0; // ¬рем€ последнего выстрела
    double currentTime = SDL_GetTicks() / 1000.0; // “екущее врем€ в секундах

    if (length <= range) {
    }
    else {
        // ¬раг продолжает движение, если он вне радиуса
        dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
        dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
        e->x += dx;
        e->y += dy;
    }

    // ѕроверка границ игрового пол€ и корректировка положени€ врага
    if (e->x < 0) e->x = 0;
    else if (e->x > WIN_WIDTH - PLAYER_WIDTH) e->x = WIN_WIDTH - PLAYER_WIDTH;

    if (e->y < 0) e->y = 0;
    else if (e->y > WIN_HEIGHT - PLAYER_HEIGHT) e->y = WIN_HEIGHT - PLAYER_HEIGHT;
    checkCollisions(es, numEnemies, current);
}
void initEnemy(Enemy* e, int type, double x, double y) // сохран€ем тип и координаты врага выбираем логику поведени€
{
    e->x = x;
    e->y = y;
    e->type = type; // —охран€ем тип в структуру дл€ отрисовки цвета и поведени€
    if (type == ENEMY_TYPE_RUNNER) {
        e->active = 1;
        e->update = updateRunningEnemyPosition;
    }
    else if (type == ENEMY_TYPE_SHOOTER) {
        e->active = 1;
        e->update = updateShootingEnemyPosition;
    }
    else if (type == ENEMY_TYPE_STATICSHOOTER)
    {
        e->active = 1;
        e->update = updatestaticShootingEnemyPosition;
    }
}
void spawnEnemies(Enemy* enemies, int numEnemies) // непосредственно выбираем тип врага, его начальный спавн и инициализируем
{
    for (int i = 0; i < numEnemies; i++) {
        int type = rand() % 3 + 1;  // —лучайный выбор типа врага который бежит или стрелка (пока так)
        int health = 100;
        double x = rand() % WIN_WIDTH;
        double y = rand() % WIN_HEIGHT;
        initEnemy(&enemies[i], type, x, y);
    }
}