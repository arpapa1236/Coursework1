#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SDL_ttf.h>
#include <SDL.h>
#include <stdio.h>

typedef struct Text
{
	SDL_Texture* str;
	SDL_Rect rect;
};

//Создание надписи из строки str шрифтом font с цветом color, будет отрисовываться в центре dstrect(при передаче NULL отрисовывает в левом верхнем углу)
Text* TextCreate(const char* str, SDL_Renderer* ren, SDL_Color color, TTF_Font* font);
//Отрисовка надписи text на экрна
void TextRender(Text* text, SDL_Renderer* ren);
//Освобождение памяти
void TextDestroy(Text* text);