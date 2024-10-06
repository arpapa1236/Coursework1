#include "Game.h"

void drawPlayer(Player* player)
{
    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
    SDL_Rect playerRect = { player->x, player->y, PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderFillRect(ren, &playerRect);
}
void drawEnemy(Enemy* enemy, int type) {
    //switch (type) {
    //case ENEMY_TYPE_RUNNER:
    //    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    //    break;
    //case ENEMY_TYPE_SHOOTER:
    //    SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
    //    break;
    //case ENEMY_TYPE_STATICSHOOTER:
    //    SDL_SetRenderDrawColor(ren, 255, 0, 255, 255);
    //    break;
    //default:
    //    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    //    break;
    //}
    //SDL_Rect enemyRect = { enemy->x, enemy->y, ENEMY_SIZE,ENEMY_SIZE };
    //SDL_RenderFillRect(ren, &enemyRect);
    Sprite_RenderCopy(ren, enemy->sprite);
}
void drawBoost(Boost* boost, int type)
{
    switch (type)
    {
    case SPEED_BOOST:
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // ���� �������
        break;
    case DMG_BOOST:
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255); // ���� �� ������ ��������
        break;
    case HP_BOOST:
        SDL_SetRenderDrawColor(0, 255, 0, 255);
        break;
    default:
        break;
    }
    SDL_Rect boostRect = { boost->x,boost->y, BOOST_SIZE,BOOST_SIZE };
    SDL_RenderFillRect(ren, &boostRect);
}

int Game()
{
	bool run = true;
    SDL_Event ev;
    Player player = {Sprite_Load(ren, "player.png"), WIN_WIDTH / 2, WIN_HEIGHT / 2, PLAYER_SPEED, 100, BULLET_DAMAGE};
    int currentFrameTime=0, lastFrameTime=SDL_GetTicks(), dTime = 0, lastBulletTime = 0;;
    Enemy enemies[MAX_ENEMIES];
    Weapon playerWeaponTrip;
    Weapon playerWeapon;
    Weapon enemyWeapon;
    initWeapon(&playerWeaponTrip, WEAPON_TYPE_TRIPLE_SHOT);
    initWeapon(&playerWeapon, WEAPON_TYPE_BASIC);
    initWeapon(&enemyWeapon, WEAPON_TYPE_ENEMY);
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
    int numEnemies = 5;
    spawnEnemies(enemies, numEnemies);
    Boost boost;
    boost.active = false;
    boost.spawnTime = SDL_GetTicks() + (rand() % 2 + 1); //boost.spawnTime = SDL_GetTicks() + (rand() % (MAX_SPAWN_TIME - MIN_SPAWN_TIME + 1) + MIN_SPAWN_TIME);
        while (run) // ������� ������� ����
        {
            while (SDL_PollEvent(&ev)) // ����� ����� ����� ����� ����� ��� ��������� �����.
            {
                switch (ev.type)
                {
                case SDL_QUIT:
                    run = false;
                    break;
                }
            }
            currentFrameTime = SDL_GetTicks();
            dTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            WASDmovement(&player, dTime);


            if (!boost.active && currentFrameTime >= boost.spawnTime) // ���� �� ������� � ����� ���� ������� ������� �������
            {
                spawnBoost(&boost, currentFrameTime);
                boost.active = true;
            }
            updateBoost(&boost, currentFrameTime); // ������������ ���� ����� ��������, ���� �� �������� ����� ���� ���� (���������!!!)
            if (checkBoostCollisionWithPlayer(&player, &boost)) // ���� ��������� ����� ��������� � ��������� �� ������ ����� �������� ��������������� � ������� ������� � ����� ��� �� �����
            {
                boost.active = false;
                applyBoost(&player, &boost);
            }
            removeBoost(&player, &boost, currentFrameTime); // ���� ���� ������� �� ������ �� ������� ��� �� ��������� �������
            for (int i = 0; i < numEnemies; i++) {
                if(enemies[i].health>0)
                enemies[i].update(&enemies[i], &player, enemies, dTime, numEnemies, i); // ��������� �������� ������� ������
            }
            //updateRunningEnemyPosition(&Runningenemy, &player, dTime);
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderClear(ren);
            Textur_RenderCopy(ren, texturs[map_background]);
            if (SDL_GetTicks() >= playerWeapon.nextFireTime) {
                playerWeapon.fire(&player, bullets, MAX_BULLETS, enemies, numEnemies);
                playerWeapon.nextFireTime = SDL_GetTicks() + (1000 / playerWeapon.fireRate);
            }
            if (SDL_GetTicks() >= playerWeaponTrip.nextFireTime) {
                playerWeaponTrip.fire(&player, bullets, MAX_BULLETS, enemies, numEnemies);
                playerWeaponTrip.nextFireTime = SDL_GetTicks() + (1000 / playerWeaponTrip.fireRate);
            }
            if (SDL_GetTicks() >= enemyWeapon.nextFireTime) {
                for (int i = 0; i < numEnemies; i++) {
                    if (enemies[i].active && enemies[i].type != ENEMY_TYPE_RUNNER) {
                        enemyWeapon.enfire(&enemies[i], bullets, MAX_BULLETS, &player, numEnemies);
                    }
                }
                enemyWeapon.nextFireTime = SDL_GetTicks() + (1000 / enemyWeapon.fireRate);
            }
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    bullets[i].x += bullets[i].dx * (dTime / 1000.0);
                    bullets[i].y += bullets[i].dy * (dTime / 1000.0);

                    // �������� �� ����� �� ������� ������
                    if (bullets[i].x < 0 || bullets[i].x > WIN_WIDTH || bullets[i].y < 0 || bullets[i].y > WIN_HEIGHT) {
                        bullets[i].active = false;
                    }
                    else {
                        if (bullets[i].owner == OWNER_PLAYER) {
                            // �������� ������������ � ������
                            for (int j = 0; j < MAX_ENEMIES; j++) {
                                if (enemies[j].active && checkBulletCollision(bullets[i], enemies[j])) {
                                    bullets[i].active = false;
                                    enemies[j].health -= player.dmg;
                                    if (enemies[j].health > 0)
                                        enemies[j].active = 1;
                                    else
                                        enemies[j].active = 0; //���� � ������� ���
                                    break;
                                }
                            }
                        }
                        else if (bullets[i].owner == OWNER_ENEMY) {
                            // �������� ������������ � �������
                            if (checkEnemyBulletCollision(bullets[i], player)) {
                                bullets[i].active = false;
                                player.health -= BULLET_DAMAGE;
                                // ����� ����� ���� ������ ��������� ����� ������
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    drawBullet(ren, &bullets[i]);
                }
            }
            drawPlayer(&player);
            for (int i = 0; i < numEnemies; i++) {
                drawEnemy(&enemies[i], enemies[i].type);
            }
            if (boost.active) // ���� ��������� ������������
                drawBoost(&boost, boost.type);
#pragma region HealthBar

#pragma endregion
            SDL_RenderPresent(ren);

        }
    return 0;
}	
