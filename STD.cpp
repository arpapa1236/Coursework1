#include "STD.h"

/*
1. Фон главного меню
2. Фон карты
3. Гробик
*/
const char* texturs_str[TEXTURS] =
{
	{"bac_menu.png"},
	{"bac_game.png"},
	{"boost1.png"},
	{"boost2.png"},
	{"boost3.png"},
	{"grob.png"}
};

const char* sprite_str[SPRITS] =
{
	{"runner.spr"},
	{"shooter.spr"},
	{"staticshooter.spr"},
	{"boss.spr"},
	{"player.spr"}
};

void Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		printf("Инициализация SDL прошла с ошибками:%s\n", SDL_GetError());
		exit(0b0010);
	}

	win = SDL_CreateWindow("The best Vampire Survivors", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == NULL)
	{
		printf("Ошибка создания окна:%s\n", SDL_GetError());
		SDLDeInit(0b0010);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL)
	{
		printf("Ошибка создания рендера:%s\n", SDL_GetError());
		SDLDeInit(0b0010);
	}

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
	{
		printf("Инициализация SDL_Image прошла с ошибками:%s\n", IMG_GetError());
		SDLDeInit(0b0010);
	}

	if (Mix_OpenAudio(48000, AUDIO_U8, 1, 2048))
	{
		printf("Ошибка инициализации аудио системы: %s\n", Mix_GetError());
		SDLDeInit(0b0010);
	}

	if (TTF_Init() != NULL)
	{
		printf("Ошибка инициализации ttf:%s\n", TTF_GetError());
		SDLDeInit(0b0010);
	}
}

void SDLDeInit(int error)
{
	if (ren) SDL_DestroyRenderer(ren);
	if (win) SDL_DestroyWindow(win);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(error);
}

void Load()
{
	keyboard = SDL_GetKeyboardState(NULL);
	music = Mix_LoadMUS("music.mp3");
	texturs = (Textur**)malloc(sizeof(Textur*) * TEXTURS);
	sprits = (Sprite**)malloc(sizeof(Sprite*) * SPRITS);
	records = RecordsLoad();
	for (int i = 0; i < TEXTURS; i++)
	{
		texturs[i] = TexturLoad(ren, texturs_str[i]);
	}
	for (int i = 0; i < SPRITS; i++)
	{
		sprits[i] = SpriteLoad(ren, sprite_str[i]);
	}
}

void MemoryFree()
{
	Mix_FreeMusic(music);
	for (int i = 0; i < TEXTURS; i++) TexturDestroy(texturs[i]);
	for (int i = 0; i < SPRITS; i++) SpriteFree(sprits[i]);
	RecordsDestroy(records);
}