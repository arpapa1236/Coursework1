#pragma once
#include "Sprite.h"
#include "Textur.h"
#include "Record.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_mixer.h>
/*Клавиатура*/
extern const Uint8* keyboard;
/*Окно*/
extern SDL_Window* win;
/*Рендер*/
extern SDL_Renderer* ren;
/*Все текстуры*/
extern Textur** texturs;
/*Все спрайты*/
extern Sprite** sprits;
/*Единый шрифт*/
extern TTF_Font* fond;
/*Массив со всеми рекордами*/
extern Record* records;
/*Музыка*/
extern Mix_Music* music;

#define TEXTUR_MULT 0.5

//Обратная совместимость
#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 50
#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50

#define MAX_ENEMIES 100
#define ENEMY_TYPE_RUNNER 1
#define ENEMY_TYPE_SHOOTER 2
#define ENEMY_TYPE_STATICSHOOTER 3
#define ENEMY_SIZE 50
#define PLAYER_SPEED 90
#define ENEMY_SPEED 45
#define BULLET_SPEED 150
#define BULLET_SIZE 3
#define BULLET_DAMAGE 5
#define MAX_BULLETS 500
#define BOOST_SIZE 40
#define SPEED_BOOST 1
#define SPEED_BOOST_AMOUNT 50
#define DMG_BOOST 2
#define DMG_BOOST_AMOUNT 2
#define BOOST_DURATION 15000
#define BOOST_PLAYER_DURATION 15000
#define MIN_SPAWN_TIME 20000
#define MAX_SPAWN_TIME 21000
#define WEAPON_TYPE_BASIC 1
#define WEAPON_TYPE_TRIPLE_SHOT 2
#define WEAPON_TYPE_AUTOAIM 3
#define WEAPON_TYPE_ENEMY 4

typedef enum {
    OWNER_PLAYER,
    OWNER_ENEMY
} BulletOwner;

struct Player {
    Sprite* text;
    double x, y;
    int speed, health, dmg, boostType, boostEndTime;
};

struct Enemy {
    Sprite* sprite;
    double x, y;
    int health;
    int type;
    bool active;
    void (*update)(void*, void*, void*, double, int, int); // функция обновления для игрового цикл, нашел, вроде прикольно, до конца все равно не понимаю
};

struct Boost {
    double x, y;
    int type, spawnTime;
    bool active;
};

typedef struct {
    double x, y;
    double dx, dy;  // Направление движения
    int type;
    bool active;
    Enemy* target;// Указатель на цель для автонаведения
    BulletOwner owner;
} Bullet;

typedef struct {
    int type;
    double fireRate;
    double nextFireTime;
    void (*fire)(void* owner, Bullet* bullets, int maxBullets, Enemy* enemies, int numEnemies);
    void (*enfire)(void* owner, Bullet* bullets, int maxBullets, Player* player, int numEnemies);
} Weapon;

extern Bullet bullets[MAX_BULLETS];