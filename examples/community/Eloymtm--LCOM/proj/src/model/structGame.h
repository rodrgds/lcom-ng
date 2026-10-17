#ifndef STRUCTGAME_H
#define STRUCTGAME_H

#include "board.h"
#include "../state/state.h"

typedef struct {
    STATE state;
    Board *board1;
    Board *board2;
    bool isPlayer1;
} Game;

#endif // STRUCTGAME_H
