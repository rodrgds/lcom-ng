#ifndef __SPRITES_H
#define __SPRITES_H

#include <lcom/lcf.h>
#include "video.h"
#include "xpms/mainMenu.h"
#include "xpms/spaceship.h"
#include "xpms/asteroid.h"
#include "xpms/cursor.h"
#include "xpms/bullet.h"

void draw_main_menu();
void draw_help_menu();
void draw_spaceship(int x);
void draw_asteroid(int x, int y);
void draw_mouse();
void draw_bullet(int x, int y);
void draw_score(int score, int x, int y);
void draw_game_over();
void draw_time(int time);

#endif 
