#include "Enemies.h"
#include "Global.h"
void checkCollisions(Enemy* enemies, int numEnemies, int current)//  оллизи€ врагов между собой
// ѕо диагонали проблемно потом доработать об€зательно что тут что на игроке, на пуле мб тоже
{
    for (int i = 0; i < numEnemies; i++) {
        int w = enemies[i].sprite->src.w/2;
        int h = enemies[i].sprite->src.h/2;
        int max = h > w ? h : w;
        if (i != current && enemies[i].active) {
            double dx = enemies[i].x - enemies[current].x;
            double dy = enemies[i].y - enemies[current].y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < max) {
                enemies[current].x -= dx / distance * (w - distance) / 2;
                enemies[current].y -= dy / distance * (h - distance) / 2;
                enemies[i].x += dx / distance * (w - distance) / 2;
                enemies[i].y += dy / distance * (h - distance) / 2;
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
    if (dx < 0)
        e->IsLeft = 1;
    else if (dx > 0)
        e->IsLeft = 0;
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
    e->sprite->dst.x = e->x;
    e->sprite->dst.y = e->y;
}
void updateShootingEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current) {
    Enemy* e = (Enemy*)enemy;
    Player* p = (Player*)player;
    Enemy* es = (Enemy*)enemies;
    double dx = p->x - e->x;
    double dy = p->y - e->y;
    if (dx < 0)
        e->IsLeft = 1;
    else if (dx > 0)
        e->IsLeft = 0;
    double length = sqrt(dx * dx + dy * dy);
    double range = 75; // ƒопустимый радиус, в пределах которого враг не будет приближатьс€ к игроку
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
    e->sprite->dst.x = e->x;
    e->sprite->dst.y = e->y;
}
void updatestaticShootingEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current) {
    Enemy* e = (Enemy*)enemy;
    Player* p = (Player*)player;
    Enemy* es = (Enemy*)enemies;
    double dx = p->x - e->x;
    double dy = p->y - e->y;
    if (dx < 0)
        e->IsLeft = 1;
    else if (dx > 0)
        e->IsLeft = 0;
    double length = sqrt(dx * dx + dy * dy);
    double range = 150;
    static double lastShootTime = 0;
    double currentTime = SDL_GetTicks() / 1000.0;

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
    e->sprite->dst.x = e->x;
    e->sprite->dst.y = e->y;
}
void updateEnemyBoss(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current)
{
    Enemy* e = (Enemy*)enemy;
    Player* p = (Player*)player;
    Enemy* es = (Enemy*)enemies;
    double dx = p->x - e->x;
    double dy = p->y - e->y;
    if (dx < 0)
        e->IsLeft = 1;
    else if (dx > 0)
        e->IsLeft = 0;
    double length = sqrt(dx * dx + dy * dy);
    double range = 250;
    static int startChargeTime = 0;
    if (e->attack)
    {
        if (SDL_GetTicks() - startChargeTime > CHARGE_DURATION * 1000)
        {
            dx = e->target_x - e->x;
            dy = e->target_y - e->y;
            length = sqrt(dx * dx + dy * dy);
            dx = (dx / length) * CHARGE_SPEED * (dTime / 1000.0);
            dy = (dy / length) * CHARGE_SPEED * (dTime / 1000.0);
            e->x += dx;
            e->y += dy;
            if (length<1)
                e->attack = false;
        }
    }
    else
    {
        if (length <= range)
        {
            startChargeTime = SDL_GetTicks();
            e->attack = true;
            e->target_x = p->x;
            e->target_y = p->y;
        }
        else {
            // ¬раг продолжает движение, если он вне радиуса
            dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
            dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
            e->x += dx;
            e->y += dy;
        }
    }

    // ѕроверка границ игрового пол€ и корректировка положени€ врага
    if (e->x < 0) e->x = 0;
    else if (e->x > WIN_WIDTH - PLAYER_WIDTH) e->x = WIN_WIDTH - PLAYER_WIDTH;

    if (e->y < 0) e->y = 0;
    else if (e->y > WIN_HEIGHT - PLAYER_HEIGHT) e->y = WIN_HEIGHT - PLAYER_HEIGHT;
    checkCollisions(es, numEnemies, current);
    e->sprite->dst.x = e->x;
    e->sprite->dst.y = e->y;
}
void initEnemy(Enemy* e, int type, double x, double y, int numOfWave) // сохран€ем тип и координаты врага выбираем логику поведени€
{
    e->x = x;
    e->y = y;
    e->type = type; // —охран€ем тип в структуру дл€ отрисовки цвета и поведени€
    e->active = 1;
    e->IsLeft = 1;
    if (type == ENEMY_TYPE_RUNNER) {
        e->health = 35+numOfWave*5;
        e->update = updateRunningEnemyPosition;
        e->sprite = SpriteCopy(sprits[sprite_enemy_runner]);
    }
    else if (type == ENEMY_TYPE_SHOOTER) {
        e->health = 10 + numOfWave * 5;
        e->update = updateShootingEnemyPosition;
        e->sprite = SpriteCopy(sprits[sprite_enemy_shooter]);
    }
    else if (type == ENEMY_TYPE_STATICSHOOTER)
    {
        e->health = 10 + numOfWave * 5;
        e->update = updatestaticShootingEnemyPosition;
        e->sprite = SpriteCopy(sprits[sprite_enemy_staticshooter]);
    }
    else if (type == ENEMY_TYPE_BOSS)
    {
        e->health = 200;
        e->update = updateEnemyBoss;
        e->sprite = SpriteCopy(sprits[sprite_enemy_boss]);
        e->attack = false;
    }
    e->dead = TexturCopy(texturs[textur_grob]);
    e->sprite->dst.h *= TEXTUR_MULT;
    e->sprite->dst.w *= TEXTUR_MULT;
    e->sprite->dst.x = x;
    e->sprite->dst.y = y;
}
void deleteEnemies(Enemy* enemies, int numEnemies)
{
    for (int i = 0; i < numEnemies; i++)
    {
        free(enemies[i].sprite);
        free(enemies[i].dead);
    }
}
void spawnEnemies(Enemy* enemies, int numEnemies, int numOfWave) // непосредственно выбираем тип врага, его начальный спавн и инициализируем
{
    /*deleteEnemies(enemies, numEnemies - 10);*/
    if (numOfWave < 4)
    {
        for (int i = 0; i < numEnemies; i++)
        {
            int type = rand() % numOfWave+1;  // —лучайный выбор типа врага который бежит или стрелка (пока так)
            double x = rand() % WIN_WIDTH;
            double y = rand() % WIN_HEIGHT + HEIGHT_HEALTH_BAR;
            initEnemy(&enemies[i], type, x, y, numOfWave);
        }
    }
    else if (numOfWave==4)
    {
        int type = 4;
        double x = rand() % WIN_WIDTH;
        double y = rand() % WIN_HEIGHT;
        initEnemy(&enemies[0], type, x, y, numOfWave);
    }
}