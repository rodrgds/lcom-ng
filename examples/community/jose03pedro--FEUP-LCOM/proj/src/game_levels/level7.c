/**
 * @file level7.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "level7.h"

int level7_draw_counter = 0;
extern PlayerPosition playerPosition;
extern PlayerPosition copy;
Box finish_level7;
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
extern Sprite_t *level7_tag;
extern Sprite_t *lock;
extern Sprite_t *key;
extern Box lock_cube;
extern bool key_check;

void update_player_starting_position7() {
  playerPosition.x = 567;
  playerPosition.y = 271;
  copy.x = 567;
  copy.y = 271;
}

void draw_game_level7() {
  memset(&ice_cubes, 0, sizeof(ice_cubes));
  path_counter = 0;

  if (level7_draw_counter == 0) {
    memset(&water_path, 0, sizeof(water_path));
    update_player_starting_position7();
    water_counter = 0;
    key_check = false;
    lock_cube.top_left_x = 417;
    lock_cube.top_left_y = 181;
    lock_cube.bot_right_x = 417 + 30;
    lock_cube.bot_right_y = 181 + 30;
  }

  int x_;
  int y_;
  draw_sprite(level7_tag, 135, 32);
  Box ic;
  int ice_cube_counter = 0;

  x_ = 567;
  y_ = 301;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 597;
  y_ = 301;
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

  x_ = 627;
  y_ = 181;
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

  x_ = 657;
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

  x_ = 627;
  y_ = 91;
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

  x_ = 597;
  y_ = 61;
  for (int x = 0; x < 15; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ -= 30;
  }

  x_ = 177;
  y_ = 91;
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

  x_ = 207;
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
  x_ = 237;
  y_ = 331;
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 297;
  y_ = 361;
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
  x_ = 357;
  y_ = 391;
  for (int x = 0; x < 7; x++) {
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
  for (int y = 0; y < 7; y++) {
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
  y_ = 181;
  for (int x = 0; x < 3; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ -= 30;
  }
  x_ = 447;
  y_ = 211;
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
  x_ = 417;
  y_ = 241;
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
  x_ = 387;
  y_ = 211;
  for (int x = 0; x < 5; x++) {
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
  y_ = 181;
  for (int x = 0; x < 5; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ -= 30;
  }
  x_ = 357;
  y_ = 271;
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
  x_ = 417;
  y_ = 301;
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
  x_ = 417;
  y_ = 331;
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
  x_ = 237;
  y_ = 121;
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
  ic.top_left_x = 597;
  ic.top_left_y = 91;
  ic.bot_right_x = 597 + 30;
  ic.bot_right_y = 91 + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, 597, 91);
  ice_cube_counter++;

  // PATH CUBES
  x_ = 207;
  y_ = 91;
  for (int x = 0; x < 13; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  draw_sprite(path_cube, 207, 121);
  path_counter++;

  x_ = 387;
  y_ = 121;
  for (int x = 0; x < 9; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 207;
  y_ = 151;
  for (int x = 0; x < 15; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 207;
  y_ = 181;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 207;
  y_ = 211;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 207;
  y_ = 241;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 207;
  y_ = 271;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 207;
  y_ = 301;
  for (int x = 0; x < 7; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  draw_sprite(path_cube, 207, 331);
  path_counter++;
  x_ = 207;
  y_ = 361;
  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 267;
  y_ = 331;
  for (int x = 0; x < 5; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 387;
  y_ = 361;
  for (int x = 0; x < 5; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 387;
  y_ = 271;
  for (int x = 0; x < 5; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  draw_sprite(path_cube, 507, 301);
  draw_sprite(path_cube, 507, 331);
  draw_sprite(path_cube, 507, 211);
  draw_sprite(path_cube, 507, 241);
  draw_sprite(path_cube, 477, 211);
  draw_sprite(path_cube, 477, 241);
  path_counter += 6;
  x_ = 567;
  y_ = 271;
  for (int y = 0; y < 5; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ -= 30;
  }
  draw_sprite(path_cube, 417, 181);
  path_counter++;

  // check if player is on top of the key
  if (playerPosition.x == 507 && playerPosition.y == 271) {
    key_check = true;
  }

  // check if player has picked key
  if (!key_check) {
    draw_sprite(key, 507, 271);
    draw_sprite(lock, 417, 181);
  }
  else {
    memset(&lock_cube, 0, sizeof(lock_cube));
  }

  // draw red cube
  x_ = 417;
  y_ = 211;
  finish_level7.top_left_x = 417;
  finish_level7.top_left_y = 211;
  finish_level7.bot_right_x = 417 + 30;
  finish_level7.bot_right_y = 211 + 30;
  draw_sprite(red_cube, x_, y_);
  path_counter--;

  // player
  draw_sprite(player, playerPosition.x, playerPosition.y);

  ice_melt();

  // check if player has completed the path
  if (playerPosition.x == finish_level7.top_left_x && playerPosition.y == finish_level7.top_left_y) {
    if (check_all_ice_melted()) {
      gameLevel = LEVEL_8;
    }
    else {
      return;
    }
  }

  level7_draw_counter++;
  check_fell_into_water();
}
