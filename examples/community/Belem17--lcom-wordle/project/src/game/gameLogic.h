#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include <lcom/lcf.h>
#include "../utils/structs.h"

void stateChanger(struct Game *game);
void gameHandler(struct Game *game);
void wordle_trace(char target[5], char guess[5], int trace[5]);
int chooseWord(struct Game *game);
void normalize_target(char *target);

#endif
