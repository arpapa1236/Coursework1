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

Button* ButtonCreate(Textur* textur, const char* text, SDL_Rect rect_button, SDL_Color color_button);

void ButtonDrav(Button* but);

void ButtonFree(Button* but);