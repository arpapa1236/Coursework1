#pragma once
void spawnBoost(Boost* boost, int currentTime);
void updateBoost(Boost* boost, int currentTime);
bool checkBoostCollisionWithPlayer(Player* player, Boost* boost);
void applyBoost(Player* player, Boost* boost);
void removeBoost(Player* player, Boost* boost, int currentTime);