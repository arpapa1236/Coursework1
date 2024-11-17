#include "Game.h"
void Win(int time)
{
	Text *textwin = TextCreate("You win!!", ren, { 0, 0, 255, 0 }, fond);
	textwin->rect.x = (WIN_WIDTH - textwin->rect.w) / 2;
	textwin->rect.y = (WIN_HEIGHT - textwin->rect.h) / 2;
	int beginTime = SDL_GetTicks(), newTine = beginTime;
	while(newTine - beginTime < 3000){
		TextRender(textwin, ren);
		SDL_RenderPresent(ren);
		newTine = SDL_GetTicks();
		SDL_PollEvent(NULL);
	}
	TextDestroy(textwin);

	char name[10];
	IfRecord(records, time);
	printf("New rocord!!\n");
	printf("Name record>");
	scanf("%s", name);
	Record* newRocord = RecordCreate(name, time);
	NewRecord(records, newRocord);
	free(newRocord);
	RecordsSave(records);
	RecordsPrint(records);
}

int* x, r;

void CircleCreate(int rad)
{
	r = rad;
	x = (int*)malloc(sizeof(int) * (r + 1));
	for (int i = 0; i <= r; i++)
		x[i] = r * cos(asin((double)i / r));
}

void Circle(Player* player)
{
	SDL_Point center = {player->x + player->text->dst.w / 2, player->y + player->text->dst.h / 2 };
	SDL_SetRenderDrawColor(ren, 75, 100, 60, 0);
	for (int i = 0; i <= r; i++)
	{
		SDL_RenderDrawLine(ren, center.x - x[i], center.y + i, center.x + x[i], center.y + i);
		SDL_RenderDrawLine(ren, center.x - x[i], center.y - i, center.x + x[i], center.y - i);
	}
}

void CircleDestroi()
{
	free(x);
}

void Loss()
{
	Text* textwin = TextCreate("You loss :-(", ren, { 0, 0, 255, 0 }, fond);
	textwin->rect.x = (WIN_WIDTH - textwin->rect.w) / 2;
	textwin->rect.y = (WIN_HEIGHT - textwin->rect.h) / 2;
	int beginTime = SDL_GetTicks(), newTine = beginTime;
	while (newTine - beginTime < 3000) {
		TextRender(textwin, ren);
		SDL_RenderPresent(ren);
		newTine = SDL_GetTicks();
		SDL_PollEvent(NULL);
	}
	TextDestroy(textwin);
}
//Полоска здоровья
void HealthBar(Player* player)
{
	SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
	SDL_Rect rect = { 0, 0, WIN_WIDTH, HEIGHT_HEALTH_BAR };
	SDL_RenderFillRect(ren, &rect);
	rect.x = 10;
	rect.y = 10;
	rect.h = HEIGHT_HEALTH_BAR - 20;
	rect.w -= 20;
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderFillRect(ren, &rect);
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
	rect.w = (WIN_WIDTH - 20) * player->health / 100;
	SDL_RenderFillRect(ren, &rect);
}

void drawPlayer(Player* player, Uint32 dtime)
{
	if (player->IsLeft)
		Sprite_RenderCopyExp(ren, player->text, 0, SDL_FLIP_HORIZONTAL);
	else
		Sprite_RenderCopy(ren, player->text);
	if (player->Run)
		Sprite_NextFrame(player->text, dtime);
}

void drawEnemy(Enemy* enemy, Uint32 dtime) {
	if (enemy->active)
	{
		if (enemy->IsLeft)
			Sprite_RenderCopyExp(ren, enemy->sprite, 0, SDL_FLIP_HORIZONTAL);
		else
			Sprite_RenderCopy(ren, enemy->sprite);
		Sprite_NextFrame(enemy->sprite, dtime);
	}
	else
		Textur_RenderCopy(ren, enemy->dead);
}

void drawBoost(Boost* boost, int type)
{
	Textur* text;
	switch (type)
	{
	case SPEED_BOOST:
		text = texturs[textur_boost1];
		break;
	case DMG_BOOST:
		text = texturs[textur_boost2];
		break;
	case HP_BOOST:
		text = texturs[textur_boost3];
		break;
	default:
		text = NULL;
		break;
	}
	text->dst.x = boost->x;
	text->dst.y = boost->y;
	Textur_RenderCopy(ren, text);
}
//Таймер в углу экрана
void Timer(TTF_Font* font, Uint32 time)
{
	char str[20];
	int sec = time / 1000;
	sprintf(str, "%2.d:%2.d.%3.d", sec / 60, sec % 60, time % 1000);
	for (int i = 0; i < 10; i++)
		if (str[i] == ' ') str[i] = '0';
	Text* timer_text = TextCreate(str, ren, { 255, 255, 255, 255 }, font);
	timer_text->rect.x = WIN_WIDTH - timer_text->rect.w - 10;
	timer_text->rect.y = WIN_HEIGHT - timer_text->rect.h - 10;
	TextRender(timer_text, ren);
	TextDestroy(timer_text);
}

int Game()
{
	CircleCreate(100);
	TTF_Font* font_timer = TTF_OpenFont("Font\\RobotoMono.ttf", 30);
	bool run = true;
	SDL_Event ev;
	Player player = { Sprite_Copy(sprits[sprite_player]), WIN_WIDTH / 2, WIN_HEIGHT / 2, PLAYER_SPEED, 100, BULLET_DAMAGE, 0, 0};
	player.text->dst.w *= 0.5;
	player.text->dst.h *= 0.5;
	player.text->dst.x = WIN_WIDTH / 2;
	player.text->dst.y = WIN_HEIGHT / 2;
	Uint32 currentFrameTime = 0, lastFrameTime = SDL_GetTicks(), dTime = 0, lastBulletTime = 0, attack1Time = 0, beginGame, endGame;
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
	int numEnemies = 0, numOfWave = 1, IswaveActive = 0, activeEnemies = 0;
	Boost boost;
	boost.active = false;
	boost.spawnTime = SDL_GetTicks() + (rand() % 2 + 1); //boost.spawnTime = SDL_GetTicks() + (rand() % (MAX_SPAWN_TIME - MIN_SPAWN_TIME + 1) + MIN_SPAWN_TIME);

	beginGame = SDL_GetTicks();
	bool quit = false;
	while (run)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				endGame = SDL_GetTicks();
				run = false;
				quit = true;
				break;
			}
		}
		player.Run = 0;
#pragma region TIME
		currentFrameTime = SDL_GetTicks();
		dTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		attack1Time += dTime;
#pragma endregion //TIME
		WASDmovement(&player, dTime);
#pragma region WAVES
		if (activeEnemies == 0)
		{
			if (numOfWave < 3)

			{
				numOfWave++;
				numEnemies += 10;
				activeEnemies += numEnemies;
			}
			else if (numOfWave < 4)
			{
				numOfWave++;
				numEnemies = 1;
				activeEnemies += numEnemies;
			}
			else
			{
				run = false;
				// you win + record
			}
			spawnEnemies(enemies, numEnemies, numOfWave);
		}

#pragma endregion
		if (!boost.active && currentFrameTime >= boost.spawnTime) // буст не активен и спавн тайм подошел спавним активим
		{
			spawnBoost(&boost, currentFrameTime);
			boost.active = true;
		}
		updateBoost(&boost, currentFrameTime); // деактивируем если долго валяется, пока не проверял может есть баги (ПРОВЕРИТЬ!!!)
		if (checkBoostCollisionWithPlayer(&player, &boost)) // если коснулись буста деактивим и применяем на игрока потом проверки непосредственно в функцию загнать в мейне они не нужны
		{
			boost.active = false;
			applyBoost(&player, &boost);
		}
		removeBoost(&player, &boost, currentFrameTime); // Если буст активен на игроке то убираем его по истечению времени
		for (int i = 0; i < numEnemies; i++) {
			if (enemies[i].health > 0)
				enemies[i].update(&enemies[i], &player, enemies, dTime, numEnemies, i); // постоянно апдейтим позиции врагов
		}
#pragma region CLEAR
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
		Textur_RenderCopy(ren, texturs[textur_map_background]);
		Circle(&player);
#pragma endregion //CLEAR
#pragma region TIMER
		Timer(font_timer, lastFrameTime - beginGame);
#pragma endregion //TIMER

		if (SDL_GetTicks() >= playerWeapon.nextFireTime && numOfWave > 1) {
			playerWeapon.fire(&player, bullets, MAX_BULLETS, enemies, numEnemies);
			playerWeapon.nextFireTime = SDL_GetTicks() + (1000 / playerWeapon.fireRate);
		}
		if (SDL_GetTicks() >= playerWeaponTrip.nextFireTime && numOfWave > 2) {
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

				// Проверка на выход за пределы экрана
				if (bullets[i].x < 0 || bullets[i].x > WIN_WIDTH || bullets[i].y < 0 || bullets[i].y > WIN_HEIGHT) {
					bullets[i].active = false;
				}
				else {
					if (bullets[i].owner == OWNER_PLAYER) {
						// Проверка столкновения с врагом
						for (int j = 0; j < numEnemies; j++) {
							if (enemies[j].active && checkBulletCollision(bullets[i], enemies[j])) {
								bullets[i].active = false;
								enemies[j].health -= player.dmg;
								if (enemies[j].health > 0)
									enemies[j].active = 1;
								else
								{
									enemies[j].health = 0;
									enemies[j].dead->dst = enemies[j].sprite->dst;
									enemies[j].active = 0;
									activeEnemies--;
								}
								break;
							}
						}
					}
					else if (bullets[i].owner == OWNER_ENEMY) {
						// Проверка столкновения с игроком
						if (checkEnemyBulletCollision(bullets[i], player)) {
							bullets[i].active = false;
							player.health -= BULLET_DAMAGE;
							// Здесь может быть логика нанесения урона игроку
						}
					}
				}
			}
		}
		if (attack1Time >= 300)
		{
			for (int j = 0; j < numEnemies; j++)
			{
				if (enemies[j].active && AreaDamage(&player, &enemies[j], 100))
				{
					enemies[j].health -= 3;
					if (enemies[j].health > 0)
						enemies[j].active = 1;
					else
					{
						enemies[j].health = 0;
						enemies[j].dead->dst = enemies[j].sprite->dst;
						enemies[j].active = 0;
						activeEnemies--;
					}
				}
				if (enemies[j].active && AreaDamageforEnemy(&player, &enemies[j], 50))
					player.health -= 2;
				if (enemies[j].active && AreaDamageforEnemy(&player, &enemies[j], 50) && numOfWave == 4)
					player.health -= 1000;
			}
			attack1Time = 0;
		}
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i].active) {
				drawBullet(ren, &bullets[i]);
			}
		}
		drawPlayer(&player, dTime);
		for (int i = 0; i < numEnemies; i++) {
			drawEnemy(&enemies[i], dTime);
		}
		if (boost.active) // если заспавнен отрисовываем
			drawBoost(&boost, boost.type);
#pragma region HEALTH_BAR
		HealthBar(&player);
#pragma endregion //HEALTH_BAR
		SDL_RenderPresent(ren);
		if (player.health <= 0)
			run = false;
	}
	if (quit)
		return -1;
	else
		if (numOfWave == 4 && player.health > 0) Win(endGame - beginGame);
		else Loss();
	free(player.text);
	return 0;
}
