#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#define FIRST_LETTER_POS_X 300
#define FIRST_LETTER_POS_Y 100
#define LETTER_SPACING 20

#define TIME_LIMIT 15;

void gameCountdown(int remaining_time);

void draw_game_over_screen();

int handle_game_input(uint8_t scancode);

void draw_word_guesses();

void draw_used_letters();

#endif 
