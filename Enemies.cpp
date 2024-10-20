#include "Enemies.h"
void checkCollisions(Enemy* enemies, int numEnemies, int current)// Коллизия врагов между собой
// По диагонали проблемно потом доработать обязательно что тут что на игроке, на пуле мб тоже
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
//Вродь норм
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
    double range = 75; // Допустимый радиус, в пределах которого враг не будет приближаться к игроку
    static double lastShootTime = 0;
    double currentTime = SDL_GetTicks() / 1000.0;

    // Определение, должен ли враг двигаться или остановиться
    if (length > range) {
        dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
        dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
        e->x += dx;
        e->y += dy;
    }
    else {
        // Враг останавливается на границе радиуса
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
    static double lastShootTime = 0; // Время последнего выстрела
    double currentTime = SDL_GetTicks() / 1000.0; // Текущее время в секундах

    if (length <= range) {
    }
    else {
        // Враг продолжает движение, если он вне радиуса
        dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
        dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
        e->x += dx;
        e->y += dy;
    }

    // Проверка границ игрового поля и корректировка положения врага
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
    e->cooldown_timer = CHARGE_COOLDOWN;
    if (dx < 0)
        e->IsLeft = 1;
    else if (dx > 0)
        e->IsLeft = 0;
    double length = sqrt(dx * dx + dy * dy);
    if (e->cooldown_timer > 0) {
        e->cooldown_timer -= dTime / 1000.0;
    }
    if (e->charge_timer > 0) {
        e->charge_timer -= dTime / 1000.0;
        if (e->charge_timer <= 0) {
            e->is_charging = true;
            e->target_x = p->x;
            e->target_y = p->y;
        }
        return;
    }
    if (e->is_charging) {
        double charge_dx = e->target_x - e->x;
        double charge_dy = e->target_y - e->y;
        double charge_length = sqrt(charge_dx * charge_dx + charge_dy * charge_dy);

        if (charge_length > 1.0) {
            charge_dx = (charge_dx / charge_length) * CHARGE_SPEED * (dTime / 1000.0);
            charge_dy = (charge_dy / charge_length) * CHARGE_SPEED * (dTime / 1000.0);
            e->x += charge_dx;
            e->y += charge_dy;
        }
        else {
            e->is_charging = false;
            e->cooldown_timer = CHARGE_COOLDOWN;
        }
        return;
    }

    // Если расстояние до игрока меньше CHARGE_DISTANCE и кулдаун завершен
    if (length <= CHARGE_DISTANCE && e->cooldown_timer <= 0) {
        // Начинаем задержку перед тараном
        e->charge_timer = CHARGE_DURATION;
    }
    else 
    {
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
}
void initEnemy(Enemy* e, int type, double x, double y, int numOfWave) // сохраняем тип и координаты врага выбираем логику поведения
{
    e->x = x;
    e->y = y;
    //e->x = x;
    //e->y = y;
    e->type = type; // Сохраняем тип в структуру для отрисовки цвета и поведения
    e->active = 1;
    e->IsLeft = 1;
    if (type == ENEMY_TYPE_RUNNER) {
        e->health = 35+numOfWave*5;
        e->update = updateRunningEnemyPosition;
        e->sprite = Sprite_Load(ren, "runner.spr");
    }
    else if (type == ENEMY_TYPE_SHOOTER) {
        e->health = 10 + numOfWave * 5;
        e->update = updateShootingEnemyPosition;
        e->sprite = Sprite_Load(ren, "shooter.spr");
    }
    else if (type == ENEMY_TYPE_STATICSHOOTER)
    {
        e->health = 10 + numOfWave * 5;
        e->update = updatestaticShootingEnemyPosition;
        e->sprite = Sprite_Load(ren, "staticshooter.spr");
    }
    else if (type == ENEMY_TYPE_BOSS)
    {
        e->health = 200;
        e->update = updateEnemyBoss;
        e->sprite = Sprite_Load(ren, "staticshooter.spr");
    }
    e->sprite->dst.h *= TEXTUR_MULT;
    e->sprite->dst.w *= TEXTUR_MULT;
    e->sprite->dst.x = x;
    e->sprite->dst.y = y;
