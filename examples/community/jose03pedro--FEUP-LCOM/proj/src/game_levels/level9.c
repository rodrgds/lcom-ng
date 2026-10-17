/**
 * @file level9.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "level9.h"

int level9_draw_counter = 0;
extern PlayerPosition playerPosition;
extern PlayerPosition copy;
Box finish_level9;
extern Box ice_cubes[250];
extern Box water_path[250];
extern int water_counter;
extern int path_counter;
extern GameLevel gameLevel;
extern Sprite_t *game_screen;
extern Sprite_t *background_cube;
extern Sprite_t *ice_cube;
extern Sprite_t *red_cube;
extern Sprite_t *water;
extern Sprite_t *player;
extern Sprite_t *path_cube;
extern Sprite_t *level9_tag;
extern Sprite_t *lock;
extern Sprite_t *key;
extern Box lock_cube;
extern bool key_check;

void update_player_starting_position9() {
  playerPosition.x = 267;
  playerPosition.y = 421;
  copy.x = 267;
  copy.y = 421;
}

void draw_game_level9() {
  memset(&ice_cubes, 0, sizeof(ice_cubes));
  path_counter = 0;

  if (level9_draw_counter == 0) {
    memset(&water_path, 0, sizeof(water_path));
    update_player_starting_position9();
    water_counter = 0;
    key_check = false;
    lock_cube.top_left_x = 597;
    lock_cube.top_left_y = 151;
    lock_cube.bot_right_x = 597 + 30;
    lock_cube.bot_right_y = 151 + 30;
  }

  // draw level9 tag on the top left corner
  draw_sprite(level9_tag, 135, 32);

  // draw level9 ice cubes
  int x_ = 117;
  int y_ = 61;
  int ice_cube_counter = 0;
  Box ic;
  for (int x = 0; x < 19; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 117;
  y_ = 91;
  for (int y = 0; y < 14; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 537;
  y_ = 121;
  for (int y = 0; y < 2; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 657;
  y_ = 91;
  for (int y = 0; y < 14; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 117;
  y_ = 481;
  for (int x = 0; x < 19; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 387;
  y_ = 91;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 477;
  y_ = 91;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 177;
  y_ = 121;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 237;
  y_ = 151;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 147;
  y_ = 301;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 207;
  y_ = 361;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 177;
  y_ = 421;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 237;
  y_ = 421;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 237;
  y_ = 451;
  for (int x = 0; x < 3; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 297;
  y_ = 421;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 357;
  y_ = 421;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 537;
  y_ = 391;
  for (int x = 0; x < 4; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 537;
  y_ = 361;
  for (int x = 0; x < 2; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 567;
  y_ = 211;
  for (int x = 0; x < 3; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 567;
  y_ = 181;
  for (int y = 0; y < 3; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 627;
  y_ = 151;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  // draw level9 path cubes

  x_ = 147;
  y_ = 91;
  for (int x = 0; x < 8; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 417;
  y_ = 91;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 507;
  y_ = 91;
  for (int x = 0; x < 5; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 147;
  y_ = 121;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 207;
  y_ = 121;
  for (int x = 0; x < 11; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 597;
  y_ = 121;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 147;
  y_ = 151;
  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 267;
  y_ = 151;
  for (int x = 0; x < 9; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 597;
  y_ = 151;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 147;
  y_ = 181;
  for (int y = 0; y < 4; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 597;
  y_ = 181;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 567;
  y_ = 241;
  for (int x = 0; x < 3; x++) {
    y_ = 241;
    for (int y = 0; y < 4; y++) {
      draw_sprite(path_cube, x_, y_);
      path_counter++;
      y_ += 30;
    }
    x_ += 30;
  }

  x_ = 177;
  y_ = 181;
  for (int x = 0; x < 13; x++) {
    y_ = 181;
    for (int y = 0; y < 6; y++) {
      draw_sprite(path_cube, x_, y_);
      y_ += 30;
      path_counter++;
    }
    x_ += 30;
  }

  x_ = 147;
  y_ = 331;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 147;
  y_ = 361;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 237;
  y_ = 361;
  for (int x = 0; x < 10; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 597;
  y_ = 361;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 147;
  y_ = 391;
  for (int x = 0; x < 13; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 147;
  y_ = 421;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 207;
  y_ = 421;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 267;
  y_ = 421;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 327;
  y_ = 421;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 387;
  y_ = 421;
  for (int x = 0; x < 9; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 147;
  y_ = 451;
  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 327;
  y_ = 451;
  for (int x = 0; x < 11; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 627;
  y_ = 181;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  // draw level9 red cube
  x_ = 627;
  y_ = 181;
  finish_level9.top_left_x = 627;
  finish_level9.top_left_y = 181;
  finish_level9.bot_right_x = 627 + 30;
  finish_level9.bot_right_y = 181 + 30;
  draw_sprite(red_cube, x_, y_);
  path_counter--;

  // check if player is on top of the key
  if (playerPosition.x == 147 && playerPosition.y == 331) {
    key_check = true;
  }

  // check if player has picked key
  if (!key_check) {
    draw_sprite(key, 147, 331);
    draw_sprite(lock, 597, 151);
  }
  else {
    memset(&lock_cube, 0, sizeof(lock_cube));
  }

  // draw player
  draw_sprite(player, playerPosition.x, playerPosition.y);

  ice_melt();

  // check if player has completed the path
  if (playerPosition.x == finish_level9.top_left_x && playerPosition.y == finish_level9.top_left_y) {
    if (check_all_ice_melted()) {
      gameLevel = LEVEL_10;
    }
    else {
      return;
    }
  }

  level9_draw_counter++;
  check_fell_into_water();
}
