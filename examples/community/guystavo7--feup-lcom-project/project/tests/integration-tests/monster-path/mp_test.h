#ifndef GAME
#define GAME
#include "../../../../lab5/spriteXMP.h"
#include "../../../include/inputHandler.h"
#include "../../../resources/1.h"
#include "../../../include/data_structures/vector.h"
#include "../../../include/monster/monster.h"
#include "../../../include/monster/path.h"

#include <stdio.h>

void startGame();
void exitGame();
void gameLoop(Terminal* terminal,float delta_time);

#endif
