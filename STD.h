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
*/
enum Texturs
{
	main_menu_background,
	map_background,
	boost1,
	boost2,
	boost3
};
//Колличество текстур
#define TEXTURS 5

/* спрайты
	1. Игрое
	2. Оружие 1
	3. Оружие 2
	4. Оружие 3
	5. Враг 1
	6. Враг 2
	7. Враг 3
	8. Враг 4
*/
enum Sprits
{
	player,
	weapon1,
	weapon2,
	weapon3,
	enemy1,
	enemy2,
	enemy3,
	enemy4
};
//Колличество спрайтов
#define SPRITS 8

//Размеры окна
#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

/*Инициализирует SDL*/
void Init();
/*Загрузка игры*/
void Load();
/*Деинициализирует SDL, принимает код ошибки*/
void SDL_DeInit(int error);