#ifndef __GAME_H_
#define __GAME_H_

#include "timer.h"
#include "KBC.h"
#include "KBC_mouse.h"
#include "RTC.h"
#include "video_gr.h"

#include "graphics.h"
#include "sprites.h"
#include "entities.h"
#include "input.h"
#include "collisions.h"
#include "scores.h"
#include "Serial.h"

#define FPS 60

#define STARTING_LIVES 2
#define STARTING_LEVEL 0

#define SCORE0 150
#define SCORE1 500
#define SCORE2 1500

int init();
int gameTick();
int gameDraw();
int stop();

void gameWin();
int isGameOver();
void gameLose();

int initBackground();
void drawBackground();

int add_screenShake(int ss);

extern char* filepath;

#endif
