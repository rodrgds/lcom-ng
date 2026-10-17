/**
 * @file game_logic.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "game_logic.h"

extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern uint8_t byte_i;
extern uint8_t scancode;
GameState gameState = RUNNING;
MenuState menuState = START;
GameLevel gameLevel;

extern uint8_t *main_fb;
extern uint8_t *secondary_fb;
extern uint32_t fb_size;

extern Box ice_cubes[250];
extern Box lock_cube;

Sprite_t *mouse;
Sprite_t *start_screen;
Sprite_t *start_button;
Sprite_t *quit_button;
Sprite_t *game_screen;
Sprite_t *background_cube;
Sprite_t *ice_cube;
Sprite_t *red_cube;
Sprite_t *water;
Sprite_t *player;
Sprite_t *path_cube;
Sprite_t *level1_tag;
Sprite_t *level2_tag;
Sprite_t *level3_tag;
Sprite_t *level4_tag;
Sprite_t *level5_tag;
Sprite_t *level6_tag;
Sprite_t *level7_tag;
Sprite_t *level8_tag;
Sprite_t *level9_tag;
Sprite_t *level10_tag;
Sprite_t *end_screen;
Sprite_t *finish_button;
Sprite_t *lock;
Sprite_t *key;

// Posição do Jogador
PlayerPosition playerPosition;
PlayerPosition copy;

int timer_counter = 0;

void sprites_creation() {
  mouse = sprite_create((xpm_map_t) mouse_xpm);
  start_screen = sprite_create((xpm_map_t) thin_ice_start_xpm);
  start_button = sprite_create((xpm_map_t) start_button_xpm);
  quit_button = sprite_create((xpm_map_t) quit_button_xpm);
  background_cube = sprite_create((xpm_map_t) background_cube_xpm);
  game_screen = sprite_create((xpm_map_t) thin_ice_game_xpm);
  ice_cube = sprite_create((xpm_map_t) ice_cube_xpm);
  red_cube = sprite_create((xpm_map_t) red_cube_xpm);
  water = sprite_create((xpm_map_t) water_xpm);
  player = sprite_create((xpm_map_t) player_xpm);
  path_cube = sprite_create((xpm_map_t) path_cube_xpm);
  level1_tag = sprite_create((xpm_map_t) level1_xpm);
  level2_tag = sprite_create((xpm_map_t) level2_xpm);
  level3_tag = sprite_create((xpm_map_t) level3_xpm);
  level4_tag = sprite_create((xpm_map_t) level4_xpm);
  level5_tag = sprite_create((xpm_map_t) level5_xpm);
  level6_tag = sprite_create((xpm_map_t) level6_xpm);
  level7_tag = sprite_create((xpm_map_t) level7_xpm);
  level8_tag = sprite_create((xpm_map_t) level8_xpm);
  level9_tag = sprite_create((xpm_map_t) level9_xpm);
  level10_tag = sprite_create((xpm_map_t) level10_xpm);
  end_screen = sprite_create((xpm_map_t) end_screen_xpm);
  finish_button = sprite_create((xpm_map_t) finish_button_xpm);
  lock = sprite_create((xpm_map_t) lock_xpm);
  key = sprite_create((xpm_map_t) key_xpm);
}

void update_timer() {
  memcpy(main_fb, secondary_fb, fb_size); // swap buffers
  timer_counter++;
}

void update_keyboard() {
  kbc_ih();
  switch (scancode) {
    case Q: // if player wants to close game
      gameState = EXIT;
      break;
    case H: // if player wants to return to main menu
      menuState = START;
      reset_all_draw_counters();
      break;
    case G: // if player wants to start over from the first level
      menuState = GAME;
      gameLevel = LEVEL_1;
      reset_all_draw_counters();
      break;
    case E:
      menuState = FINISH;
      reset_all_draw_counters();
    case A:
      if (menuState == GAME) {
        bool check = true;
        for (int i = 0; i < 250; i++) {
          if (ice_cubes[i].top_left_x == playerPosition.x - 30 && ice_cubes[i].top_left_y == playerPosition.y) {
            check = false;
            break;
          }
        }
        if (lock_cube.top_left_x == playerPosition.x - 30 && lock_cube.top_left_y == playerPosition.y) {
          check = false;
        }
        if (check) {
          playerPosition.x -= 30;
        }
      }
      break;
    case W:
      if (menuState == GAME) {
        bool check = true;
        for (int i = 0; i < 250; i++) {
          if (ice_cubes[i].top_left_x == playerPosition.x && ice_cubes[i].top_left_y == playerPosition.y - 30) {
            check = false;
            break;
          }
        }
        if (lock_cube.top_left_x == playerPosition.x && lock_cube.top_left_y == playerPosition.y - 30) {
          check = false;
        }
        if (check)
          playerPosition.y -= 30;
      }
      break;
    case S:
      if (menuState == GAME) {
        bool check = true;
        for (int i = 0; i < 250; i++) {
          if (ice_cubes[i].top_left_x == playerPosition.x && ice_cubes[i].top_left_y == playerPosition.y + 30) {
            check = false;
            break;
          }
        }
        if (lock_cube.top_left_x == playerPosition.x && lock_cube.top_left_y == playerPosition.y + 30) {
          check = false;
        }
        if (check)
          playerPosition.y += 30;
      }
      break;
    case D:
      if (menuState == GAME) {
        bool check = true;
        for (int i = 0; i < 250; i++) {
          if (ice_cubes[i].top_left_x == playerPosition.x + 30 && ice_cubes[i].top_left_y == playerPosition.y) {
            check = false;
            break;
          }
        }
        if (lock_cube.top_left_x == playerPosition.x + 30 && lock_cube.top_left_y == playerPosition.y) {
          check = false;
        }
        if (check)
          playerPosition.x += 30;
      }
      break;
    default:
      break;
  }
  draw_new_screen();
}

void update_mouse() {
  (mouse_ih)();
  mouse_sync_bytes();
  if (byte_i == 3) {
    mouse_sync_info();
    update_start_and_finish_button();
    draw_new_screen();
    byte_i = 0;
  }
}

void update_start_and_finish_button() {
  if (mouse_info.left_click) {
    if (menuState == START) {
      if (mouse_info.x >= 260 && mouse_info.x <= 385 && mouse_info.y >= 485 && mouse_info.y <= 524) {
        start_button->is_clicked = 1;
        menuState = GAME;
        gameLevel = LEVEL_1;
        reset_all_draw_counters();
      }
      if (mouse_info.x >= 415 && mouse_info.x <= 540 && mouse_info.y >= 485 && mouse_info.y <= 524) {
        quit_button->is_clicked = 1;
        gameState = EXIT;
      }
    }
    if (menuState == FINISH) {
      if (mouse_info.x >= 340 && mouse_info.x <= 465 && mouse_info.y >= 467 && mouse_info.y <= 506) {
        finish_button->is_clicked = 1;
        menuState = START;
        reset_all_draw_counters();
      }
    }
  }
  else {
    start_button->is_clicked = 0;
    quit_button->is_clicked = 0;
    finish_button->is_clicked = 0;
  }
}
