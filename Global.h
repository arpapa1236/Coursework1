#pragma once
#include "Sprite.h"
#include "Textur.h"
#include "Record.h"
#include <SDL_image.h>
/*Клавиатура*/
extern const Uint8* keyboard;
/*Окно*/
extern SDL_Window* win;
/*Рендер*/
extern SDL_Renderer* ren;
/*Все текстуры*/
extern Textur** texturs;
/*Все спрайты*/
extern Sprite** sprits;
/*Единый шрифт*/
extern TTF_Font* fond;
/*Массив со всеми рекордами*/
extern Record* records;
/*Музыка*/
extern Mix_Music* music;