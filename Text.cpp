#include "Text.h"

Text* TextCreate(const char* str, SDL_Renderer* ren, SDL_Color color, TTF_Font* font)
{
	SDL_Surface* syrf = TTF_RenderUTF8_Blended(font, str, color);
	if (syrf == NULL)
	{
		printf("Error ttf:%s\n", SDL_GetError());
		exit(-100);
	}
	Text* rez = (Text*)malloc(sizeof(Text));

	rez->rect = { 0 , 0, syrf->w, syrf->h };

	rez->str = SDL_CreateTextureFromSurface(ren, syrf);
	SDL_FreeSurface(syrf);

	return rez;
}

void TextRender(Text* text, SDL_Renderer* ren)
{
	SDL_RenderCopy(ren, text->str, NULL, &text->rect);
}

void TextDestroy(Text* text)
{
	SDL_DestroyTexture(text->str);
	free(text);
}