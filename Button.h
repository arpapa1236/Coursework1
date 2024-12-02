#pragma once
#include "STD.h"
#include "Text.h"

struct Button
{
	Textur* textur;
	Text* text;
	SDL_Rect rect;
	SDL_Color color;
};

Button* ButtonCreate(const Textur* textur, const char* text, SDL_Rect rect_button, SDL_Color color_button);

void ButtonDrav(Button* but);

void ButtonFree(Button* but);

/*
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
	Player player = { Sprite_Copy(sprits[sprite_player]), WIN_WIDTH / 2, WIN_HEIGHT / 2, PLAYER_SPEED, 100, BULLET_DAMAGE, 0, 0, 0, 0, {0, 0, 0} };
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
#pragma region Button
	int actButton;
	Button** buttons = 0;
	SDL_Point mouse = { 0,0 };
	bool runButton;
	SDL_Rect buttonRect[3] = { {100, 200, 200, 300}, {400, 200, 200, 300}, {700, 200, 200, 300} };
	const char* buttonString[3] = {"1", "2", "3"};
	SDL_Color buttonColor = { 100, 100, 100, 0 };
	buttons = (Button**)malloc(sizeof(Button*) * 3);
	for (int i = 0; i < 3; i++)
		buttons[i] = ButtonCreate(NULL, buttonString[i], buttonRect[i], buttonColor);
#pragma endregion //Button
	Boost boost;
	boost.active = false;
	boost.spawnTime = SDL_GetTicks() + (rand() % 2 + 1); //boost.spawnTime = SDL_GetTicks() + (rand() % (MAX_SPAWN_TIME - MIN_SPAWN_TIME + 1) + MIN_SPAWN_TIME);
	beginGame = SDL_GetTicks();
	bool quit = false;
	while (run)
	{
		runButton = true;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				endGame = SDL_GetTicks();
				run = false;
				quit = true;
				break;
			case SDL_MOUSEMOTION:
				mouse.x = ev.motion.x;
				mouse.y = ev.motion.y;
				if(buttons) actButton = SDL_PointInRect(&mouse, &buttons[0]->rect) ? 1 : (SDL_PointInRect(&mouse, &buttons[1]->rect) ? 2 : (SDL_PointInRect(&mouse, &buttons[2]->rect) ? 3 : 0));
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == 1)
					if (actButton > 0)
					{
						player.weapons[actButton - 1]++;
						runButton = false;
						printf("%d\n", actButton);
					}
			}
		}
#pragma region CLEAR
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
		Textur_RenderCopy(ren, texturs[textur_map_background]);
#pragma endregion //CLEAR
#pragma region BUTTON
		for(int i = 0; i < 3; i++)
			ButtonDrav(buttons[i]);
#pragma endregion //BUTTON
		SDL_RenderPresent(ren);
	}
	if (quit)
		return -1;
	else
		if (numOfWave == 4 && player.health > 0) Win(endGame - beginGame);
		else Loss();
	free(player.text);
	return 0;
}

*/