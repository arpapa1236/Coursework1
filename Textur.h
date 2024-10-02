#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
typedef struct Textur
{
	SDL_Texture* text;
	SDL_Rect dst, src;
} Textur;
/*Загрузка текстуры описанной в file. Возвращает структуру типа Textur*/
Textur* Textur_Load(SDL_Renderer* ren, const char* file);
/*Отрисовка текстуры из textur с помощью ren*/
void Textur_RenderCopy(SDL_Renderer* ren, Textur* textur);
/*Освобождение памяти*/
void TexturDestroy(Textur* textur);