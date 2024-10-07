#define _CRT_SECURE_NO_WARNINGS
#include "Sprite.h"

Sprite* Sprite_Load(SDL_Renderer* ren, const char* filename)
{
	char s[256] = "Sprite\\";
	strcat(s, filename);
	Sprite* rez = (Sprite*)malloc(sizeof(Sprite));
	FILE* file = fopen(s, "r");
	if (file == NULL)
	{
		printf("Ошибка открытия файла:%s\n", s);
		exit(-1);
	}
	fscanf(file, "%d %d\n%d\n%s", &rez->column, &rez->rows, &rez->speed, s + 7);
	fclose(file);

	SDL_Surface* image = IMG_Load(s);
	if (image == NULL)
	{
		printf("Ошибка загрузки спрайта(%s): %s\n", s, SDL_GetError());
		exit(-1);
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 255, 255, 255));
	rez->spr = SDL_CreateTextureFromSurface(ren, image);
	if (rez->spr == NULL)
	{
		printf("Ошибка создания спрайта(%s): %s\n", s, SDL_GetError());
		exit(-1);
	}
	rez->w = image->w;
	rez->h = image->h;
	rez->src = { 0, 0, rez->w / rez->column, rez->h / rez->rows };
	rez->dst = { 0, 0, rez->w / rez->column, rez->h / rez->rows };
	SDL_FreeSurface(image);

	rez->framex = rez->framey = rez->frametime = 0;
	return rez;
}

void Sprite_NextFrame(Sprite* spr, int dtime)
{
	spr->frametime += dtime;
	if (spr->frametime >= 1000 / spr->speed)
	{
		spr->framex++;
		if (spr->framex == spr->column)
		{
			spr->framex = 0;
			spr->framey++;
			if (spr->framey == spr->rows)
				spr->framey = 0;
		}
		spr->src = { spr->framex * spr->w / spr->column, spr->framey * spr->h / spr->rows, spr->src.w, spr->src.h };
		spr->frametime = 0;
	}
}

void Sprite_RenderCopy(SDL_Renderer* ren, Sprite* spr)
{
	if(SDL_RenderCopy(ren, spr->spr, &spr->src, &spr->dst))
		printf("Ошибка отрисовки:%s\n", SDL_GetError());
}

void Sprite_RenderCopyExp(SDL_Renderer* ren, Sprite* spr, double angel, SDL_RendererFlip flag)
{	
	if (SDL_RenderCopyEx(ren, spr->spr, &(spr->src), &(spr->dst), angel, NULL, flag))
		printf("Ошибка отрисовки:%s\n", SDL_GetError());
}

void Sprite_Free(Sprite* spr)
{
	SDL_DestroyTexture(spr->spr);
	free(spr);
}

//Немецкия классическая философия как культурный феномен. Общие черты