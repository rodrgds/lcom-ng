#pragma once
#include "cursor.h"


uint8_t bit_no_kbd;
uint8_t bit_no_mouse;
uint8_t bit_no_timer;
uint8_t bit_no_rtc;


#define PI 3.14159265
typedef enum {
    MENU,
    PLAYING,
    EXIT
} GameState; 

typedef struct {
    //cursor *gameCursor;
    GameState gState;

} Game;

typedef struct {
    int speedX,
        speedY,
        posX,
        posY,
        x,
        y,
        id;
    
} Bullet;

typedef struct {
    int x,
        y;
    bool alive;
} Block;

Block blocks[256];
Bullet *bullet;

Game* Start();

GameState getGameState(Game* game);

void Handler(Game* game);

int End();

void process_mouse_event(Game* game, struct packet* pp);

void clearArrow();
void underArrow();

void saveBoard();
void drawBoard();

void updateScreen();
void drawMousePull();
void drawBullet();
void drawBlocks();
void shootBullet(int pullX, int pullY);

void paintCell();


void nextLevel();


