#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Global.h"
#include "Textur.h"

/* текстуры
1.	Фон главного меню
2.	Фон карты
3.  Враг 1
4. Враг 2
5. Враг 3
6. Враг 4
7. Враг 5
*/
enum Texturs
{
	main_menu_background,
	map_background,
	enemy1,
	enemy2,
	enemy3,
	enemy4,
	enemy5,
};
//Колличество текстур
#define TEXTURS 7

/* спрайты
	1. Игрое
	2. Оружие 1
	3. Оружие 2
	4. Оружие 3
	5. Оружие 4
	6. Оружие 5
*/
enum Sprits
{
	player,
	weapon1,
	weapon2,
	weapon3,
	weapon4,
	weapon5
};
//Колличество спрайтов
#define SPRITS 6

//Размеры окна
#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

/*Инициализирует SDL*/
void Init();
/*Загрузка игры*/
void Load();
/*Деинициализирует SDL, принимает код ошибки*/
void SDL_DeInit(int error);