/**
 * @file level10.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "level10.h"

int level10_draw_counter = 0;
extern PlayerPosition playerPosition;
extern PlayerPosition copy;
Box finish_level10;
extern Box ice_cubes[250];
extern Box water_path[250];
extern int water_counter;
extern int path_counter;
extern MenuState menuState;
extern Sprite_t *game_screen;
extern Sprite_t *background_cube;
extern Sprite_t *ice_cube;
extern Sprite_t *red_cube;
extern Sprite_t *water;
extern Sprite_t *player;
extern Sprite_t *path_cube;
extern Sprite_t *level10_tag;
extern Sprite_t *lock;
extern Sprite_t *key;
extern Box lock_cube;
extern bool key_check;

void update_player_starting_position10() {
  playerPosition.x = 207;
  playerPosition.y = 271;
  copy.x = 207;
  copy.y = 271;
}
void draw_game_level10() {
  memset(&ice_cubes, 0, sizeof(ice_cubes));
  path_counter = 0;

  if (level10_draw_counter == 0) {
    memset(&water_path, 0, sizeof(water_path));
    update_player_starting_position10();

    water_counter = 0;
    key_check = false;
    lock_cube.top_left_x = 207;
    lock_cube.top_left_y = 121;
    lock_cube.bot_right_x = 207 + 30;
    lock_cube.bot_right_y = 121 + 30;
  }

  draw_sprite(level10_tag, 135, 32);

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

  x_ = 117;
  y_ = 91;
  for (int y = 0; y < 13; y++) {
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
  for (int y = 0; y < 13; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 627;
  y_ = 91;
  for (int y = 0; y < 13; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 597;
  y_ = 91;
  for (int y = 0; y < 13; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 567;
  y_ = 91;

  for (int y = 0; y < 10; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }
  x_ = 477;
  y_ = 451;

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
  y_ = 91;

  for (int y = 0; y < 10; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 507;
  y_ = 91;

  for (int y = 0; y < 10; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }
  x_ = 177;
  y_ = 91;
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
  x_ = 147;
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

  x_ = 297;
  y_ = 91;
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
  y_ = 121;
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

  x_ = 177;
  y_ = 211;
  for (int y = 0; y < 8; y++) {
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
  y_ = 241;
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

  x_ = 207;
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

  x_ = 237;
  y_ = 391;

  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 357;
  y_ = 211;

  x_ = 267;
  y_ = 361;
  for (int y = 0; y < 4; y++) {
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
  y_ = 331;
  for (int y = 0; y < 5; y++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    y_ += 30;
  }

  x_ = 297;
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

  x_ = 447;
  y_ = 331;

  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 237;
  y_ = 361;

  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 327;
  y_ = 181;

  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 147;
  y_ = 91;

  draw_sprite(path_cube, x_, y_);
  path_counter++;
  x_ = 147;
  y_ = 121;
  for (int x = 0; x < 5; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 237;
  y_ = 91;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 387;
  y_ = 91;
  for (int x = 0; x < 4; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 387;
  y_ = 121;
  for (int y = 0; y < 7; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }
  x_ = 417;
  y_ = 121;
  for (int y = 0; y < 12; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }
  x_ = 447;
  y_ = 121;
  for (int y = 0; y < 7; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 447;
  y_ = 361;
  for (int y = 0; y < 4; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }
  x_ = 477;
  y_ = 121;
  for (int y = 0; y < 11; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 507;
  y_ = 391;
  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 507;
  y_ = 421;
  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 327;
  y_ = 211;
  for (int y = 0; y < 8; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }
  x_ = 357;
  y_ = 181;
  for (int y = 0; y < 9; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 297;
  y_ = 181;
  for (int y = 0; y < 9; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }
  x_ = 267;
  y_ = 151;
  for (int y = 0; y < 7; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 237;
  y_ = 151;
  for (int y = 0; y < 3; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }
  x_ = 207;
  y_ = 181;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }
  x_ = 177;
  y_ = 181;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 147;
  y_ = 181;
  for (int y = 0; y < 10; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 207;
  y_ = 271;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 207;
  y_ = 331;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 207;
  y_ = 361;

  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 207;
  y_ = 421;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 177;
  y_ = 451;
  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }
  x_ = 207;
  y_ = 391;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  if (playerPosition.x == 567 && playerPosition.y == 391) {
    key_check = true;
  }

  // check if player has picked key
  if (!key_check) {
    draw_sprite(key, 567, 391);
    draw_sprite(lock, 207, 121);
  }
  else {
    memset(&lock_cube, 0, sizeof(lock_cube));
  }

  // draw red cube
  x_ = 147;
  y_ = 91;
  finish_level10.top_left_x = 147;
  finish_level10.top_left_y = 91;
  finish_level10.bot_right_x = 147 + 30;
  finish_level10.bot_right_y = 91 + 30;
  draw_sprite(red_cube, x_, y_);
  path_counter--;

  draw_sprite(player, playerPosition.x, playerPosition.y);

  ice_melt();

  // check if player has completed the path
  if (playerPosition.x == finish_level10.top_left_x && playerPosition.y == finish_level10.top_left_y) {
    if (check_all_ice_melted()) {
      menuState = FINISH;
    }
    else {
      return;
    }
  }

  level10_draw_counter++;
  check_fell_into_water();
}
