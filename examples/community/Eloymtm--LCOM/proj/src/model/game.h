#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdlib.h>

#include "board.h"
#include "../state/state.h"
#include "../viewer/boardViewer.h"
#include "../labs/keyboard.h"
#include "../labs/mouse.h"
#include "lcom/timer.h"
#include "../controller/devicesController.h"
#include "structGame.h"
#include "../xpm/mouse1.h"

Game* (StartGame)(STATE state);

int (GameLoop)(Game *game);

#endif // GAME_H
