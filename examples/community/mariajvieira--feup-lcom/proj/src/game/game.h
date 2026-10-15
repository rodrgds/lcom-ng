#ifndef GAME_H
#define GAME_H
#include <lcom/lcf.h>

void game_start(void);
void update_game(void);
void draw_game(void);
void handle_key(uint8_t scancode);
void draw_game_over(void);

void draw_score(void);
void game_exit(void);
bool game_is_running(void);
void draw_game_static(void);
void draw_game_dynamic(void);

#endif // GAME_H
