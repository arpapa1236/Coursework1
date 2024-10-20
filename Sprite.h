#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

/*
Структура файла .spr:
{
	column rows
	speed
}
column - колличество кадров по столбцам
rows - колличество кадров по строчкам
speed - коллчиество кадров в секунду
*/

/*
	* spr - ссылка на спрайт
	* w, h - ширина и высота спрайта
	* rows, column - количество кадров в одной строке и коллчиество строк
	* src - область спрайта которую собираемся отрисвоать
	* dst - область окна в которую нужно отрисовать кадр
	* framex, framey - кадр по счету в столбце и строке
	* frametime - время текущего кадра
	* speed - скорость смены кадра в кадрах в секунду
*/
typedef struct Sprite {
	SDL_Texture* spr;
	int w, h, rows, column;
	int framex, framey, frametime, speed;
	SDL_Rect src, dst;
} Sprite;
//Загружает спрайт из файла filename передается название в папке Sprite
Sprite* Sprite_Load(SDL_Renderer* ren, const char* filename);
//Считает нужно ли поменять кадр в анимации speed - FPS
void Sprite_NextFrame(Sprite* text, int dtime);
//Обертка RenderCopy
void Sprite_RenderCopy(SDL_Renderer* ren, const Sprite* spr);
//Обертка RenderCopyExp
void Sprite_RenderCopyExp(SDL_Renderer* ren, const Sprite* spr, double angel, SDL_RendererFlip flag);
//Удаляет текстуру
void Sprite_Free(Sprite* spr);
//Делает копию обьекта Sprite возвращает ссылку на созданный объект
Sprite* Sprite_Copy(const Sprite* spr);