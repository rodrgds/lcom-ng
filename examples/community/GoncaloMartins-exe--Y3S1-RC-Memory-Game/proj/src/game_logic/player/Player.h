#ifndef PLAYER_H
#define PLAYER_H

#include "../../controllers/video/graphic.h"
typedef struct {
    int xPos, yPos;
} Player;

void player_init(Player* player);
void player_draw(const Player* player);

void player_move_up(Player* player);
void player_move_down(Player* player);
void player_move_left(Player* player);
void player_move_right(Player* player);

#endif
