#ifndef __GAME_H
#define __GAME_H

#include <lcom/lcf.h>
#include "info.h"
#include "graphics/video.h"

typedef enum {
  GAME_STATE,
  EXIT_STATE,
  GAME_OVER_STATE,
  MENU_STATE,
  HELP_STATE
} game_state_t;

void (process_mouse_packet)();
void (keyboard_handler)();
void (mouse_handler)();
int (play_pressed)();
int (exit_pressed)();
int (help_pressed)();

void(spaceship_move)(uint8_t array[], int tam);
void(timer_handler)();
void(move_asteroid)();

void(draw_all_bullets)();
void(fire_bullet)();
void(move_bullets)();

void(draw_all_asteroids)();
void(create_asteroid)();
void(move_asteroids)();

void(check_collision)();

void(deactivate_all_bullets)();
void(deactivate_all_asteroids)();

int(is_game_over)();

#endif
