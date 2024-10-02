#pragma once
struct Player {
    Sprite* text;
    double x, y;
    int speed, health, dmg, boostType, boostEndTime;
};
void movePlayer(Player* player, double dx, double dy, int dTime);
void WASDmovement(Player* player, double dTime);