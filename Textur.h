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
/*�������� �������� ��������� � file. ���������� ��������� ���� Textur*/
Textur* TexturLoad(SDL_Renderer* ren, const char* file);
/*��������� �������� �� textur � ������� ren*/
void TexturRenderCopy(SDL_Renderer* ren, const Textur* textur);
/*������������ ������*/
void TexturDestroy(Textur* textur);
//��� �������������� Textur_Copy ��� ������������� ����� ������������ free()
Textur* TexturCopy(const Textur* textur);