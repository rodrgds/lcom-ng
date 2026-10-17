#ifndef _GAME_VIEWER_H_
#define _GAME_VIEWER_H_
#include "../../utils/structs.h"
#include <stdio.h>


void showGame(struct Game *game);
void showWin(struct Game *game);
void showLose(struct Game *game);
void showLetter(char c, int x, int y);

#endif
