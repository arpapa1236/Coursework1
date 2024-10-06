#include "Enemies.h"
void checkCollisions(Enemy* enemies, int numEnemies, int current)// �������� ������ ����� �����
// �� ��������� ��������� ����� ���������� ����������� ��� ��� ��� �� ������, �� ���� �� ����
{
    for (int i = 0; i < numEnemies; i++) {
        int w = enemies[i].sprite->src.w/2;
        int h = enemies[i].sprite->src.h/2;
        int max = h > w ? h : w;
        if (i != current) {
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
void updateRunningEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current) //������
//����� ����
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
    e->sprite->dst.x = e->x;
    e->sprite->dst.y = e->y;
}
void updateShootingEnemyPosition(void* enemy, void* player, void* enemies, double dTime, int numEnemies, int current) {
    Enemy* e = (Enemy*)enemy;
    Player* p = (Player*)player;
    Enemy* es = (Enemy*)enemies;
    double dx = p->x - e->x;
    double dy = p->y - e->y;
    double length = sqrt(dx * dx + dy * dy);
    double range = 150; // ���������� ������, � �������� �������� ���� �� ����� ������������ � ������
    static double lastShootTime = 0;
    double currentTime = SDL_GetTicks() / 1000.0;

    // �����������, ������ �� ���� ��������� ��� ������������
    if (length > range) {
        dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
        dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
        e->x += dx;
        e->y += dy;
    }
    else {
        // ���� ��������������� �� ������� �������
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
    static double lastShootTime = 0; // ����� ���������� ��������
    double currentTime = SDL_GetTicks() / 1000.0; // ������� ����� � ��������

    if (length <= range) {
    }
    else {
        // ���� ���������� ��������, ���� �� ��� �������
        dx = (dx / length) * ENEMY_SPEED * (dTime / 1000.0);
        dy = (dy / length) * ENEMY_SPEED * (dTime / 1000.0);
        e->x += dx;
        e->y += dy;
    }

    // �������� ������ �������� ���� � ������������� ��������� �����
    if (e->x < 0) e->x = 0;
    else if (e->x > WIN_WIDTH - PLAYER_WIDTH) e->x = WIN_WIDTH - PLAYER_WIDTH;

    if (e->y < 0) e->y = 0;
    else if (e->y > WIN_HEIGHT - PLAYER_HEIGHT) e->y = WIN_HEIGHT - PLAYER_HEIGHT;
    checkCollisions(es, numEnemies, current);
}
void initEnemy(Enemy* e, int type, double x, double y, int numOfWave) // ��������� ��� � ���������� ����� �������� ������ ���������
{
    e->x = x;
    e->y = y;
    //e->x = x;
    //e->y = y;
    e->type = type; // ��������� ��� � ��������� ��� ��������� ����� � ���������
    e->active = 1;
    if (type == ENEMY_TYPE_RUNNER) {
        e->health = 50+numOfWave*5;
        e->update = updateRunningEnemyPosition;
        e->sprite = Sprite_Load(ren, "runner.spr");
        e->sprite->dst.h *= TEXTUR_MULT;
        e->sprite->dst.w *= TEXTUR_MULT;
        e->sprite->dst.x = x;
        e->sprite->dst.y = y;
    }
    else if (type == ENEMY_TYPE_SHOOTER) {
        e->update = updateShootingEnemyPosition;
    }
    else if (type == ENEMY_TYPE_STATICSHOOTER)
    {
        e->update = updatestaticShootingEnemyPosition;
    }
}
void deleteEnemies(Enemy* enemies, int numEnemies)
{
    //for (int i = 0; i < numEnemies; i++)
        //text_Free(enemies[i].sprite);
}
void spawnEnemies(Enemy* enemies, int numEnemies, int numOfWave) // ��������������� �������� ��� �����, ��� ��������� ����� � ��������������
{
    deleteEnemies(enemies, numEnemies - 10);
    for (int i = 0; i < numEnemies; i++)
    {
        int type = rand() % 1;  // ��������� ����� ���� ����� ������� ����� ��� ������� (���� ���)
        double x = rand() % WIN_WIDTH;
        double y = rand() % WIN_HEIGHT;
        initEnemy(&enemies[i], type, x, y, numOfWave);
    }
}