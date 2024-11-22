#define _CRT_SECURE_NO_WARNINGS
/*Стандартные библиотеки*/
#include<iostream>
/*Библиотекм SDL*/
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
/*Дополнительные библиотеки*/
#include "STD.h"
#include "Text.h"
#include "Sprite.h"
#include "Global.h"
#include "Textur.h"
#include "Record.h"
#include "Game.h"
/*Клавиатура*/
const Uint8* keyboard;
/*Окно*/
SDL_Window* win = NULL;
/*Рендер*/
SDL_Renderer* ren = NULL;
/*Все текстуры*/
Textur** texturs;
/*Все спрайты*/
Sprite** sprits;
/*Единый шрифт*/
TTF_Font* fond;
/*Массив со всеми рекордами*/
Record** records;
/*Музыка*/
Mix_Music* music;

int newtime, oldtime = 0, deltime = 0;

int Menu();

int Setting();

int SDL_main(int argc, char** argv)
{
	system("chcp 65001>nul");
	Init();
	Load();
	srand(time(NULL));
	//Шрифт
	fond = TTF_OpenFont("Font/RobotoMono.ttf", 50);
	//Запуск музыки
	Mix_PlayMusic(music, -1);
	//Установка размеров фона главного экрана
	texturs[textur_main_menu_background]->dst.w = WIN_WIDTH;
	texturs[textur_main_menu_background]->dst.h = WIN_HEIGHT;
	Menu();
	MemoryFree();
	//Высвобождение памяти
	SDL_DeInit(0);
	return 0;
}

Text*** LoadTextMenu()
{
	const char *str[3] =
	{
		{"Play"},
		{"Setting"},
		{"Exit"}
	};
	SDL_Color colors[2] = {
		{255, 0, 0, 0},
		{0, 255, 0, 0}
	};
	Text*** menu;
	menu = (Text***)malloc(sizeof(Text**) * 2 * 3);
	for (int i = 0; i < 3; i++) {
		menu[i] = (Text**)malloc(sizeof(Text*) * 2);
		for (int j = 0; j < 2; j++)
		{
			menu[i][j] = TextCreate(str[i], ren, colors[j], fond);
		}
	}

	int x0, y0, maxx = 0;
	for (int i = 0, sumy = 0; ; i++)
	{
		if (i == 3)
		{
			x0 = (WIN_WIDTH - maxx) / 2;
			y0 = (WIN_HEIGHT - sumy) / 2;
			break;
		}
		maxx = menu[i][1]->rect.w > maxx ? menu[i][1]->rect.w : maxx;
		sumy += menu[i][1]->rect.h;
	}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 2; j++)
		{
			menu[i][j]->rect.x = x0 + ((maxx - menu[i][j]->rect.w) / 2);
			menu[i][j]->rect.y = y0 + menu[i][j]->rect.h * i;
		}
	return menu;
}

void PrintMenu(Text*** text, int act)
{
	switch (act)
	{
	case 1: 
		TextRender(text[0][1], ren);
		TextRender(text[1][0], ren);
		TextRender(text[2][0], ren);
		break;
	case 2:		
		TextRender(text[0][0], ren);
		TextRender(text[1][1], ren);
		TextRender(text[2][0], ren);
		break;
	case 3:
		TextRender(text[0][0], ren);
		TextRender(text[1][0], ren);
		TextRender(text[2][1], ren);
		break;
	case 0:
		TextRender(text[0][0], ren);
		TextRender(text[1][0], ren);
		TextRender(text[2][0], ren);
		break;
	}
}

int Menu()
{
	int returnCode = 0;
	int ftime = 0;
	SDL_Point mouse = { 0, 0 };
	int act = 0;
	bool run = true;
	SDL_Event ev;
	Text*** menu = LoadTextMenu();
	while (run)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				run = false;
				break;
			case SDL_MOUSEMOTION:
				mouse.x = ev.motion.x;
				mouse.y = ev.motion.y;
				act = SDL_PointInRect(&mouse, &menu[0][0]->rect) ? 1 : (SDL_PointInRect(&mouse, &menu[1][0]->rect) ? 2 : (SDL_PointInRect(&mouse, &menu[2][0]->rect) ? 3 : 0));
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(ev.button.button == 1)
					switch (act)
					{
					case 1: 
						
						switch (Game())
						{
						case -1:
							//SDL_QUIT
							run = false;
							returnCode = -1;
							break;
						case 0:
							break;
						}
						break;
					case 2:
						switch (Setting())
						{
						case -1:							
							//SDL_QUIT
							run = false;
							returnCode = -1;
							break;
						case 0:
							break;
						}
						break;
					case 3:
						run = false;
						break;
					default:
						break;
					};
			}
		}
		newtime = SDL_GetTicks();
		deltime = newtime - oldtime;
		ftime += deltime;
		oldtime = newtime;


		/*Отрисовка*/
		if(1000 / 60 < ftime)
		{
			Textur_RenderCopy(ren, texturs[textur_main_menu_background]);
			PrintMenu(menu, act);
			SDL_RenderPresent(ren);	 
			ftime = 0;
		}
	}
	for (int i = 0; i < 3; i++)
		free(menu[i]);
	return returnCode;;
}

int Setting()
{
	int ftime = 0;
	static char strVolume[] = "Volume:100%";
	static int volume = 100;
	static const char* strManual[2] =
	{
		{"To increase the volume, press up"},
		{"To turn down the volume, press down"}
	};
	SDL_Color colorManual = { 0, 0, 255, 0 };
	Text* textManual[2] =
	{
		TextCreate(strManual[0], ren, colorManual, fond),
		TextCreate(strManual[1], ren, colorManual, fond)
	};
	Text* textVolume = TextCreate(strVolume, ren, {255, 0, 0, 0}, fond);
	SDL_Point mouse = { 0, 0 };
	SDL_Event ev;
	Text* back[2] =
	{
		TextCreate("Back", ren, {255, 0, 0, 0}, fond),
		TextCreate("Back", ren, {0, 255, 0, 0}, fond)
	};
	static int y0 = (WIN_HEIGHT - textManual[0]->rect.h - textManual[1]->rect.h - textVolume->rect.h - back[0]->rect.h) / 2;
#pragma region Y
	textManual[0]->rect.y = y0;
	textManual[1]->rect.y = textManual[0]->rect.y + textManual[0]->rect.h;
	textVolume->rect.y = textManual[1]->rect.y + textManual[1]->rect.h;
	back[0]->rect.y = back[1]->rect.y = textVolume->rect.y + textVolume->rect.h;
#pragma endregion //Y
#pragma region X
	textManual[0]->rect.x = (WIN_WIDTH - textManual[0]->rect.w) / 2;
	textManual[1]->rect.x = (WIN_WIDTH - textManual[1]->rect.w) / 2;
	textVolume->rect.x = (WIN_WIDTH - textVolume->rect.w) / 2;
	back[0]->rect.x =  back[1]->rect.x = (WIN_WIDTH - back[1]->rect.w) / 2;
#pragma endregion //X
	int act = 0;
	while (true)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEMOTION:
				mouse.x = ev.motion.x;
				mouse.y = ev.motion.y;
				act = SDL_PointInRect(&mouse, &back[0]->rect) ? 1 : 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == 1)
					switch (act)
					{
					case 1:
						return 0;
					default:
						break;
					};
			case SDL_KEYDOWN:
			{
				if ((ev.key.keysym.scancode == SDL_SCANCODE_UP && volume < 100) || (ev.key.keysym.scancode == SDL_SCANCODE_DOWN && volume > 0))
				{
					if (ev.key.keysym.scancode == SDL_SCANCODE_UP) volume++;
					else volume--;
					SDL_Rect rect = textVolume->rect;
					TextDestroy(textVolume);
					sprintf(strVolume, "Volume:%3d%c", volume, '%');
					textVolume = TextCreate(strVolume, ren, { 255, 0, 0, 0 }, fond);
					textVolume->rect = rect;
				}
			}
				break;
			}
		}
		newtime = SDL_GetTicks();
		deltime = newtime - oldtime;
		ftime += deltime;
		oldtime = newtime;
		if(1000 / 60 < ftime)
		{
			Textur_RenderCopy(ren, texturs[textur_main_menu_background]);
			TextRender(textVolume, ren);
			TextRender(textManual[0], ren);
			TextRender(textManual[1], ren);
			if (act) TextRender(back[1], ren);
			else TextRender(back[0], ren);
			SDL_RenderPresent(ren);
			//Установка громкости музыки
			Mix_VolumeMusic(128 * volume / 100);
			ftime = 0;
		}
	}
	TextDestroy(textManual[0]);
	TextDestroy(textManual[1]);
	TextDestroy(textVolume);
	TextDestroy(back[0]);
	TextDestroy(back[1]);
	return 0;
}