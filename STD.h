#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Global.h"
#include "Textur.h"
#include "Record.h"
#include "Sprite.h"

/* текстуры
	1. Фон главного меню
	2. Фон карты
	3. 1 буст
	4. 2 буст
	5. 3 буст
	6. гроб
*/
enum Texturs
{
	textur_main_menu_background,
	textur_map_background,
	textur_boost1,
	textur_boost2,
	textur_boost3,
	textur_grob
};
//Колличество текстур
#define TEXTURS 6
/* спрайты
	1. 1 враг
	2. 2 враг
	3. 3 враг
	4. босс
	5. персонаж
*/
enum Sprits
{
	sprite_enemy_runner,
	sprite_enemy_shooter,
	sprite_enemy_staticshooter,
	sprite_enemy_boss,
	sprite_player
};
#define SPRITS 5
//Размеры окна
#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

/*Инициализирует SDL*/
void Init();
/*Загрузка игры*/
void Load();
/*Деинициализирует SDL, принимает код ошибки*/
void SDL_DeInit(int error);

void MemoryFree();