#include "Game.h"
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
    int numEnemies = 5; // � ���� ����� ��������� ���, ����� ������������ �� ������� ������ ������ ������������ ������ ���� ����� ��� ������ �����-�� ��������� ��������� ������������� ���-�� ������ ���������
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
                enemies[i].update(&enemies[i], &player, enemies, dTime, numEnemies, i); // ��������� �������� ������� ������
            }
            //updateRunningEnemyPosition(&Runningenemy, &player, dTime);
            SDL_SetRenderDrawColor(render, 255, 255, 255, 255); // ���� �������� �����
            SDL_RenderClear(render);
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
                    if (bullets[i].x < 0 || bullets[i].x > WINWIDTH || bullets[i].y < 0 || bullets[i].y > WINHEIGHT) {
                        bullets[i].active = false;
                    }
                    else {
                        if (bullets[i].owner == OWNER_PLAYER) {
                            // �������� ������������ � ������
                            for (int j = 0; j < MAX_ENEMIES; j++) {
                                if (enemies[j].active && checkBulletCollision(bullets[i], enemies[j])) {
                                    bullets[i].active = false;
                                    enemies[j].health -= player.dmg;
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
                    drawBullet(render, &bullets[i]);
                }
            }
            drawPlayer(&player);
            for (int i = 0; i < numEnemies; i++) {
                drawEnemy(&enemies[i], enemies[i].type);
            }
            if (boost.active) // ���� ��������� ������������
                drawBoost(&boost, boost.type);
            SDL_RenderPresent(render);

        }
    }
    return 0;
}	
