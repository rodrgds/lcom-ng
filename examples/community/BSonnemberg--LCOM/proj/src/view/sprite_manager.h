#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "sprite.h"

sprite* table;
sprite* stick1;
sprite* stick2;
sprite* ball_turn;
sprite** balls;
sprite* menu;
sprite* cursor;
sprite* about;
sprite* about_arrow;
sprite* local_game;
sprite* local_game_arrow;
sprite* online_game;
sprite* online_game_arrow;
sprite* game_header;
sprite* about_screen;
sprite** numbers;
sprite* two_points;
sprite* player1_win;
sprite* player2_win;

/**
 * @brief Load all game sprites
 * @return non-0 in case of a fail
*/
int load_sprites();

/**
 * @brief Unload all game sprites
*/
void unload_sprites();

#endif
