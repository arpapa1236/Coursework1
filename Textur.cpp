#include "Textur.h"
Textur* Textur_Load(SDL_Renderer* ren, const char* file)
{
	Textur* rez = (Textur*)malloc(sizeof(Textur));
	char s[256] = "Textur\\";
	strcat(s, file);
	SDL_Surface* image = IMG_Load(s);
	if (image == NULL)
	{
		printf("Ошибка загрузки текстуры(%s): %s\n", s, SDL_GetError());
		exit(-1);
	}
	rez->text = SDL_CreateTextureFromSurface(ren, image);
	if (rez->text == NULL)
	{
		printf("Ошибка создания тектуры(%s): %s\n", s, SDL_GetError());
		exit(-1);
	}
	rez->dst = rez->src = { 0, 0, image->w, image->h };
	SDL_FreeSurface(image);
	return rez;
}

void Textur_RenderCopy(SDL_Renderer* ren, Textur* textur)
{
	SDL_RenderCopy(ren, textur->text, &textur->src, &textur->dst);
}

void TexturDestroy(Textur* textur)
{
	SDL_DestroyTexture(textur->text);
	free(textur);
}

Textur* Textur_Copy(Textur* textur)
{
	Textur* rez = (Textur*)malloc(sizeof(Textur));
	rez->text = textur->text;
	rez->src = textur->src;
	rez->dst = textur->dst;
	return rez;
}