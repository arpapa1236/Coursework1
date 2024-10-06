#include "Global.h"
#include "STD.h"
#include "Game.h"
#include "Enemies.h"
#include "Bullets.h"
#include "Boosts.h"
void spawnBoost(Boost* boost, int currentTime)
{
    boost->x = rand() % WIN_WIDTH;
    boost->y = rand() % WIN_HEIGHT;
    boost->type = rand() % 3;
    boost->spawnTime = currentTime + (rand() % (MAX_SPAWN_TIME - MIN_SPAWN_TIME + 1) + MIN_SPAWN_TIME);
    boost->active = false;
}
void updateBoost(Boost* boost, int currentTime)
{
    if (boost->active && (currentTime > boost->spawnTime + BOOST_DURATION))
        boost->active = false;
}
bool checkBoostCollisionWithPlayer(Player* player, Boost* boost)
{
    if (boost->active)
    {
        double playerRight = player->x + PLAYER_WIDTH;
        double playerBottom = player->y + PLAYER_HEIGHT;
        double boostRight = boost->x + BOOST_SIZE;
        double boostBottom = boost->y + BOOST_SIZE;
        if (!(playerRight < boost->x || player->x > boostRight || playerBottom < boost->y || player->y > boostBottom)) {
            return true;
        }
    }
    return false;
}
void applyBoost(Player* player, Boost* boost)
{
    player->boostType = boost->type;
    switch (boost->type)
    {
    case SPEED_BOOST:
        player->speed += SPEED_BOOST_AMOUNT;
        break;
    case DMG_BOOST:
        player->dmg += DMG_BOOST_AMOUNT;
        break;
    case HP_BOOST:
        player->health += HP_BOOST_AMOUNT;
        if (player->health > 100)
            player->health = 100;
        break;
    default:
        break;
    }
    player->boostEndTime = SDL_GetTicks() + BOOST_PLAYER_DURATION;
}
void removeBoost(Player* player, Boost* boost, int currentTime)
{
    if (player->boostType != 0 && currentTime >= player->boostEndTime) {
        switch (player->boostType)
        {
        case SPEED_BOOST:
            player->speed -= SPEED_BOOST_AMOUNT;
            break;
        case DMG_BOOST:
            player->dmg -= DMG_BOOST_AMOUNT;
            break;
        default:
            break;

        }
        player->boostType = 0;
    }
}