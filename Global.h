#pragma once
#include "Sprite.h"
#include "Textur.h"
#include "Record.h"
#include <SDL_image.h>
/*����������*/
extern const Uint8* keyboard;
/*����*/
extern SDL_Window* win;
/*������*/
extern SDL_Renderer* ren;
/*��� ��������*/
extern Textur** texturs;
/*��� �������*/
extern Sprite** sprits;
/*������ �����*/
extern TTF_Font* fond;
/*������ �� ����� ���������*/
extern Record* records;
/*������*/
extern Mix_Music* music;