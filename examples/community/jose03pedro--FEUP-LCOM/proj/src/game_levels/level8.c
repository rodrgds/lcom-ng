/**
 * @file level8.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "level8.h"

int level8_draw_counter = 0;
extern PlayerPosition playerPosition;
extern PlayerPosition copy;
Box finish_level8;
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
extern Sprite_t *level8_tag;
extern Sprite_t *lock;
extern Sprite_t *key;
extern Box lock_cube;
extern bool key_check;

void update_player_starting_position8() {
  playerPosition.x = 567;
  playerPosition.y = 91;
  copy.x = 567;
  copy.y = 91;
}

void draw_game_level8() {
  memset(&ice_cubes, 0, sizeof(ice_cubes));
  path_counter = 0;

  if (level8_draw_counter == 0) {
    memset(&water_path, 0, sizeof(water_path));
    update_player_starting_position8();
    water_counter = 0;
    key_check = false;
    lock_cube.top_left_x = 627;
    lock_cube.top_left_y = 361;
    lock_cube.bot_right_x = 627 + 30;
    lock_cube.bot_right_y = 361 + 30;
  }

  int x_;
  int y_;

  // draw level8 tag
  draw_sprite(level8_tag, 135, 32);

  // ice cubes
  x_ = 117;
  y_ = 181;
  int ice_cube_counter = 0;
  Box ic;
  for (int y = 0; y < 11; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 147;
  y_ = 181;
  for (int x = 0; x < 5; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 207;
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

  x_ = 147;
  y_ = 481;
  for (int x = 0; x < 5; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 267;
  y_ = 421;
  for (int x = 0; x < 14; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  x_ = 267;
  y_ = 451;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 237;
  y_ = 391;
  for (int y = 0; y < 4; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 267;
  y_ = 391;
  for (int y = 0; y < 2; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 357;
  y_ = 391;
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

  x_ = 417;
  y_ = 211;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 447;
  y_ = 331;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 447;
  y_ = 271;
  for (int y = 0; y < 5; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 417;
  y_ = 181;
  for (int y = 0; y < 2; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 387;
  y_ = 211;
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

  x_ = 507;
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

  x_ = 537;
  y_ = 391;
  for (int y = 0; y < 5; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 507;
  y_ = 271;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 657;
  y_ = 391;
  for (int y = 0; y < 4; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 567;
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

  x_ = 597;
  y_ = 301;
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

  x_ = 597;
  y_ = 271;
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

  x_ = 597;
  y_ = 181;
  for (int y = 0; y < 4; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ -= 30;
  }

  x_ = 537;
  y_ = 151;
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

  x_ = 507;
  y_ = 151;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 597;
  y_ = 61;
  for (int x = 0; x < 6; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ -= 30;
  }

  x_ = 417;
  y_ = 91;
  for (int x = 0; x < 1; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ -= 30;
  }

  x_ = 387;
  y_ = 61;
  for (int x = 0; x < 4; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ -= 30;
  }

  x_ = 297;
  y_ = 91;
  for (int y = 0; y < 3; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 387;
  y_ = 271;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 387;
  y_ = 331;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 327;
  y_ = 331;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 267;
  y_ = 151;
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

  // draw path cubes
  x_ = 147;
  y_ = 211;
  for (int y = 0; y < 9; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 177;
  y_ = 211;
  for (int y = 0; y < 9; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 207;
  y_ = 241;
  for (int y = 0; y < 8; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 237;
  y_ = 241;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 237;
  y_ = 421;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 267;
  y_ = 241;
  for (int y = 0; y < 4; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 297;
  y_ = 181;
  for (int y = 0; y < 8; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 327;
  y_ = 271;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 327;
  y_ = 211;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 507;
  y_ = 331;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 327;
  y_ = 361;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 327;
  y_ = 181;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 447;
  y_ = 391;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 327;
  y_ = 241;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 327;
  y_ = 301;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 327;
  y_ = 91;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 357;
  y_ = 91;
  for (int y = 0; y < 10; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 387;
  y_ = 121;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 387;
  y_ = 241;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 387;
  y_ = 301;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 387;
  y_ = 361;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 417;
  y_ = 121;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 447;
  y_ = 91;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 387;
  y_ = 91;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 417;
  y_ = 241;
  for (int y = 0; y < 6; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 447;
  y_ = 121;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 447;
  y_ = 301;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 447;
  y_ = 361;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 477;
  y_ = 211;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 477;
  y_ = 91;
  for (int y = 0; y < 11; y++) {
    if (y == 4) {
      y_ += 30;
      continue;
    }
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 507;
  y_ = 91;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 507;
  y_ = 181;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 507;
  y_ = 241;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 507;
  y_ = 301;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 507;
  y_ = 361;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 537;
  y_ = 181;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 537;
  y_ = 241;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 567;
  y_ = 91;
  for (int y = 0; y < 9; y++) {
    if (y == 4) {
      y_ += 30;
      continue;
    }
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 597;
  y_ = 331;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 597;
  y_ = 391;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ -= 30;
  }

  x_ = 627;
  y_ = 331;
  for (int y = 0; y < 3; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  // draw red cube
  x_ = 567;
  y_ = 391;
  finish_level8.top_left_x = 567;
  finish_level8.top_left_y = 391;
  finish_level8.bot_right_x = 567 + 30;
  finish_level8.bot_right_y = 391 + 30;
  draw_sprite(red_cube, x_, y_);
  path_counter--;

  // check if player is on top of the key
  if (playerPosition.x == 237 && playerPosition.y == 451) {
    key_check = true;
  }

  // check if player has picked key
  if (!key_check) {
    draw_sprite(key, 237, 451);
    draw_sprite(lock, 627, 361);
  }
  else {
    memset(&lock_cube, 0, sizeof(lock_cube));
  }

  // player
  draw_sprite(player, playerPosition.x, playerPosition.y);

  ice_melt();

  // check if player has completed the path
  if (playerPosition.x == finish_level8.top_left_x && playerPosition.y == finish_level8.top_left_y) {
    if (check_all_ice_melted()) {
      gameLevel = LEVEL_9;
    }
    else {
      return;
    }
  }

  level8_draw_counter++;
  check_fell_into_water();
}
