#include "STD.h"

/*
1. Фон главного меню
2. Фон карты
3. Враг 1
4. Враг 2
5. Враг 3
6. Враг 4
7. Враг 5
*/
char texturs_str[TEXTURS][50] =
{
	{"bac_menu.png"},
	{"bac_game.jpg"},
	{"bac_menu.png"},
	{"bac_menu.png"},
	{"bac_menu.png"},
	{"bac_menu.png"},
	{"bac_menu.png"},
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
		SDL_DeInit(0b0010);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL)
	{
		printf("Ошибка создания рендера:%s\n", SDL_GetError());
		SDL_DeInit(0b0010);
	}

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
	{
		printf("Инициализация SDL_Image прошла с ошибками:%s\n", IMG_GetError());
		SDL_DeInit(0b0010);
	}

	if (Mix_OpenAudio(48000, AUDIO_U8, 1, 2048))
	{
		printf("Ошибка инициализации аудио системы: %s\n", Mix_GetError());
		SDL_DeInit(0b0010);
	}

	if (TTF_Init() != NULL)
	{
		printf("Ошибка инициализации ttf:%s\n", TTF_GetError());
		SDL_DeInit(0b0010);
	}
}

void SDL_DeInit(int error)
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
	texturs = (Textur**)malloc(sizeof(Textur**) * TEXTURS);
	records = RecordLoad();
	for (int i = 0; i < TEXTURS; i++)
	{
		texturs[i] = Textur_Load(ren, texturs_str[i]);
	}
}