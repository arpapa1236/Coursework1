#include "Game.h"
int Game()
{
	bool run = true;
    SDL_Event ev;
    Player player = {Sprite_Load(ren, "player.png"), WIN_WIDTH / 2, WIN_HEIGHT / 2, PLAYER_SPEED, 100, BULLET_DAMAGE};
    int newTime=0, oldTime=SDL_GetTicks(), dTime = 0;
	while (run) 
	{
        while (SDL_PollEvent(&ev)) // Мышку позже когда будет выбор при получении экспы.
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                run = false;
                break;
            }
        }
        newTime = SDL_GetTicks();
        dTime = newTime - oldTime;
        oldTime = newTime;
        WASDmovement(&player, dTime);
	}
    return 0;
}	
