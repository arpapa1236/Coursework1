#include "Global.h"
#include "Textur.h"
#include "STD.h"
struct Player {
    Textur* text;
    double x, y;
    int speed, health, dmg, boostType, boostEndTime;
};
void movePlayer(Player* player, double dx, double dy, double dTime) // ѕросчет скорости и ограничени€
// ограничени€ убрать если карту бесшовником.
{
    double length = sqrt(dx * dx + dy * dy);
    if (length != 0) {
        dx = (dx / length) * player->speed * (dTime / 1000.0);
        dy = (dy / length) * player->speed * (dTime / 1000.0);
    }
    player->x += dx;
    player->y += dy;
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
}