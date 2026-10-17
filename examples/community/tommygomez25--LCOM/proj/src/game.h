#pragma once

#include "i8042.h"
#include "keyboard.h"
#include "timer.h"
#include "rtc.h"

typedef enum {TIMER, KEYBOARD, MOUSE, RTC} Device;

typedef enum {MAINMENU,PLAY, LOST, SAVE, SCOREBOARD, EXIT} GameState;

#include "snake.h"
#include "menu.h"
#include "grass.h"
#include "sprites/0.xpm"
#include "sprites/1.xpm"
#include "sprites/2.xpm"
#include "sprites/3.xpm"
#include "sprites/4.xpm"
#include "sprites/5.xpm"
#include "sprites/6.xpm"
#include "sprites/7.xpm"
#include "sprites/8.xpm"
#include "sprites/9.xpm"
#include "sprites/daybackground.xpm"
#include "sprites/afternoonbackground.xpm"
#include "sprites/nightbackground.xpm"

xpm_image_t numbers_img[10];
xpm_image_t day_img;
xpm_image_t afternoon_img;
xpm_image_t night_img;

int (game_main_loop)();

void GeneralInterrupt(Device device);

struct mouse_ev * mouse_events(struct packet * pack);

void PlayInterruptHandler(Device device);


void (loadGame)();

void (draw_score)();
void draw_sky();
