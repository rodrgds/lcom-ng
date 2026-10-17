#ifndef DRAW_GAME_H_
#define DRAW_GAME_H_

#include "../controller/game_controller.h"

void draw_game_start(GameState *game_state);
void draw_input(GameState *game_state);
void clear_input(GameState *game_state);
void draw_answer(GameState *game_state , bool correct);
void draw_game_stats(GameState *game_state, int time_remaining);
void draw_game_lost();



#endif


