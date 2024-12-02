#include "Button.h"

Button* ButtonCreate(const Textur* textur, const char* text, SDL_Rect rect_button, SDL_Color color_button)
{
	Button* rez = (Button*)malloc(sizeof(Button));
	rez->textur = Textur_Copy(textur);
	rez->text = TextCreate(text, ren, { 0, 0, 0, 0 }, fond);
	rez->rect = rect_button;
	rez->color = color_button;
	int b;
	rez->textur->dst = { rect_button.x, rect_button.y, rect_button.w, rect_button.w };
	rez->text->rect.x = rect_button.x;
	rez->text->rect.y = rect_button.y + (rect_button.h - rect_button.w - rez->text->rect.h) / 2;
	return rez;
}

void ButtonDrav(Button* but)
{
	SDL_SetRenderDrawColor(ren, but->color.r, but->color.g, but->color.b, but->color.a);
	SDL_RenderFillRect(ren, &(but->rect));
	Textur_Copy(but->textur);
	TextRender(but->text, ren);
}

void ButtonFree(Button* but)
{
	TextDestroy(but->text);
	free(but->textur);
	free(but);
}