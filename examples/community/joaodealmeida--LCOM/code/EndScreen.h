#ifndef __EndScreen_H
#define __EndScreen_H

#include "vector2.h"


#define QUIT_GAME 1
#define RESTART_GAME 2

void initEndScreen();

void drawEndScreen();

void freeEndScreen();

char endGameChoice(vector2 mousePos);

#endif
