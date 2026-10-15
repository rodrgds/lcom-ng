#include "score.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "projectiles.h"
#include "sprites.h"
#include "i8042.h"
#include "alien.h"
#include "letters.h"

int alien_score_value = 100; //how many points each alien kill is worth
Score game_score;
int best_score = 0;

void init_score(Score* score_info, int initial_lives) {
  score_info->score = 0;
  score_info->lives = initial_lives;
}

void update_score(Score* score_info, int points) {
  score_info->score += points;
}

void decrement_lives(Score* score_info) {
  if (score_info->lives > 0) {
    score_info->lives -= 1;
  }
}

void draw_char(char c, uint16_t x, uint16_t y) {
  switch (c) {
    case '0': draw_zerow(x, y); break;
    case '1': draw_onew(x, y); break;
    case '2': draw_twow(x, y); break;
    case '3': draw_threew(x, y); break;
    case '4': draw_fourw(x, y); break;
    case '5': draw_fivew(x, y); break;
    case '6': draw_sixw(x, y); break;
    case '7': draw_sevenw(x, y); break;
    case '8': draw_eightw(x, y); break;
    case '9': draw_ninew(x, y); break;
    case 'S': draw_Sw(x, y); break;
    case 'C': draw_Cw(x, y); break;
    case 'O': draw_Ow(x, y); break;
    case 'R': draw_Rw(x, y); break;
    case 'E': draw_Ew(x, y); break;
    case 'L': draw_Lw(x, y); break;
    case 'I': draw_Iw(x, y); break;
    case 'V': draw_Vw(x, y); break;
    case 'B': draw_Bw(x, y); break;
    case 'T': draw_Tw(x, y); break;
    case 'M': draw_Mw(x, y); break;
    default: break;
  }
}

void draw_string(const char* str, uint16_t x, uint16_t y) {
  while (*str) {
    draw_char(*str, x, y);
    x += 16; 
    str++;
  }
}

void draw_score(Score* score_info) {
  char score_str[20];
  sprintf(score_str, "SCORE: %d", score_info->score);
  draw_string(score_str, 10, 10); 
}

void draw_final_score(Score* score_info) {
  char score_str[20];
  sprintf(score_str, "SCORE: %d", score_info->score);
  draw_string(score_str, 305, 315); 
}

void draw_lives(Score* score_info) {
  char lives_str[20];
  sprintf(lives_str, "LIVES %d", score_info->lives);
  draw_string(lives_str, 10, 30); 
}

void draw_best_score(Score* score_info) {
  if (score_info->score > best_score) {
    best_score = score_info->score;
  }
  char score_str[20];
  sprintf(score_str, "BEST %d", best_score);
  draw_string(score_str, 305, 345);
}

void draw_time_remaining(int time_remaining) {
  char time_str[10];
  sprintf(time_str, "TIME %02d", time_remaining);
  draw_string(time_str, 675, 10);
}

