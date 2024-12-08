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
/*Единый шрифт*/
extern TTF_Font* fond;
/*Массив спрайтов*/
extern Sprite** sprits;
/*Массив со всеми рекордами*/
extern Record** records;
/*Музыка*/
extern Mix_Music* music;

#define TEXTUR_MULT 0.5

//Обратная совместимость
#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 50
#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50
// босс
#define CHARGE_DURATION 0.25// в секундах, задержка перед тараном
#define CHARGE_COOLDOWN 5
#define CHARGE_SPEED 100 

#define MAX_ENEMIES 100
#define ENEMY_SIZE 50
#define PLAYER_SPEED 90
#define ENEMY_SPEED 45
#define BULLET_SPEED 150
#define BULLET_SIZE 3
#define BULLET_DAMAGE 5
#define MAX_BULLETS 1000
#define BOOST_SIZE 40
#define BOOST_DURATION 15000
#define BOOST_PLAYER_DURATION 15000
#define MIN_SPAWN_TIME 20000
#define MAX_SPAWN_TIME 21000
#define WEAPON_TYPE_BASIC 1
#define WEAPON_TYPE_TRIPLE_SHOT 2
#define WEAPON_TYPE_AUTOAIM 3
#define WEAPON_TYPE_ENEMY 4
#define HEIGHT_HEALTH_BAR 50

typedef enum {
    SPEED_BOOST,
    DMG_BOOST,
    HP_BOOST,
    NULL_BOOST
}Boosts;
#define SPEED_BOOST_AMOUNT 50
#define DMG_BOOST_AMOUNT 2
#define HP_BOOST_AMOUNT 25

typedef enum {
ENEMY_TYPE_RUNNER = 1,
ENEMY_TYPE_SHOOTER,
ENEMY_TYPE_STATICSHOOTER,
ENEMY_TYPE_BOSS
}Enemies;

typedef enum {
    OWNER_PLAYER,
    OWNER_ENEMY
} BulletOwner;

struct Player {
    Sprite* text;
    double x, y;
    int speed, health, dmg, boostType, boostEndTime;
    bool IsLeft, Run;
};

struct Enemy {
    Sprite* sprite;
    Textur* dead;
    double x, y;
    int health;
    int type;
    bool active, IsLeft;
    void (*update)(void*, void*, void*, double, int, int); // функция обновления для игрового цикл, нашел, вроде прикольно, до конца все равно не понимаю
    int target_x, target_y;
    bool attack;
};

struct Boost {
    double x, y;
    int type, spawnTime;
    bool active;
    Textur* textur;
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