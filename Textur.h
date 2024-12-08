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
Textur* TexturLoad(SDL_Renderer* ren, const char* file);
/*Отрисовка текстуры из textur с помощью ren*/
void TexturRenderCopy(SDL_Renderer* ren, const Textur* textur);
/*Освобождение памяти*/
void TexturDestroy(Textur* textur);
//При использованиее Textur_Copy для освобождениия нужно использовать free()
Textur* TexturCopy(const Textur* textur);