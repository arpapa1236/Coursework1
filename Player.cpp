#include "Global.h"
#include "Textur.h"
#include "STD.h"
#include "Player.h"
int isRight = 1;
void movePlayer(Player* player, double dx, double dy, int dTime) // ������� �������� � �����������
{
    player->Run = 1;
    double length = sqrt(dx * dx + dy * dy);
    if (length != 0) {
        dx = (dx / length) * player->speed * (dTime / 1000.0);
        dy = (dy / length) * player->speed * (dTime / 1000.0);
    }
    player->x += dx;
    player->y += dy;
    if (dx > 0)
        player->IsLeft = 0;
    if (dx < 0)
        player->IsLeft = 1;
    if (player->x < 0) {
        player->x = 0;
    }
    else if (player->x > WIN_WIDTH - player->text->dst.w) {
        player->x = WIN_WIDTH - player->text->dst.w;
    }
    if (player->y < 0) {
        player->y = 0;
    }
    else if (player->y > WIN_HEIGHT - player->text->dst.h) {
        player->y = WIN_HEIGHT - player->text->dst.h;
    }
    player->text->dst.x = player->x;
    player->text->dst.y = player->y;
}
void WASDmovement(Player* player, double dTime)
{
    if (keyboard[SDL_SCANCODE_W] && keyboard[SDL_SCANCODE_A])
    {
        movePlayer(player, -1, -1, dTime);
    }
    else if (keyboard[SDL_SCANCODE_W] && keyboard[SDL_SCANCODE_D])
    {
        movePlayer(player, 1, -1, dTime);
    }
    else if (keyboard[SDL_SCANCODE_S] && keyboard[SDL_SCANCODE_A]) {
        movePlayer(player, -1, 1, dTime);
    }
    else if (keyboard[SDL_SCANCODE_S] && keyboard[SDL_SCANCODE_D]) {
        movePlayer(player, 1, 1, dTime);
    }
    else
    {
        if (keyboard[SDL_SCANCODE_W]) {
            movePlayer(player, 0, -1, dTime);
        }
        if (keyboard[SDL_SCANCODE_S]) {
            movePlayer(player, 0, 1, dTime);
        }
        if (keyboard[SDL_SCANCODE_A]) {
            movePlayer(player, -1, 0, dTime);
            isRight = 0;
        }
        if (keyboard[SDL_SCANCODE_D]) {
            movePlayer(player, 1, 0, dTime);
            isRight = 1;
        }
    }
}