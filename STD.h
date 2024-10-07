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
	boost3,
	grob
};
//Колличество текстур
#define TEXTURS 6

//Размеры окна
#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

/*Инициализирует SDL*/
void Init();
/*Загрузка игры*/
void Load();
/*Деинициализирует SDL, принимает код ошибки*/
void SDL_DeInit(int error);