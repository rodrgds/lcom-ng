/**
 * @file level4.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "level4.h"

int level4_draw_counter = 0;
extern PlayerPosition playerPosition;
extern PlayerPosition copy;
Box finish_level4;
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
extern Sprite_t *level4_tag;

void update_player_starting_position4() {
  playerPosition.x = 207;
  playerPosition.y = 271;
  copy.x = 207;
  copy.y = 271;
}

void draw_game_level4() {
  memset(&ice_cubes, 0, sizeof(ice_cubes));
  path_counter = 0;

  if (level4_draw_counter == 0) {
    memset(&water_path, 0, sizeof(water_path));
    update_player_starting_position4();
    water_counter = 0;
  }

  int x_;
  int y_;

  // draw level4 tag
  draw_sprite(level4_tag, 135, 32);

  // ice cubes
  x_ = 177;
  y_ = 301;
  int ice_cube_counter = 0;
  Box ic;
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

  x_ = 147;
  y_ = 211;
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

  x_ = 177;
  y_ = 91;
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

  x_ = 267;
  y_ = 121;
  for (int x = 0; x < 8; x++) {
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
  y_ = 151;
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

  x_ = 447;
  y_ = 151;
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

  x_ = 477;
  y_ = 91;
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

  x_ = 597;
  y_ = 121;
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

  x_ = 567;
  y_ = 211;
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

  x_ = 507;
  y_ = 211;
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

  x_ = 477;
  y_ = 211;
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
  y_ = 241;
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

  x_ = 417;
  y_ = 301;
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

  x_ = 237;
  y_ = 271;
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

  x_ = 297;
  y_ = 271;
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

  x_ = 237;
  y_ = 211;
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

  ic.top_left_x = 537;
  ic.top_left_y = 301;
  ic.bot_right_x = 537 + 30;
  ic.bot_right_y = 301 + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, 537, 301);
  ice_cube_counter++;

  ic.top_left_x = 537;
  ic.top_left_y = 151;
  ic.bot_right_x = 537 + 30;
  ic.bot_right_y = 151 + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, 537, 151);
  ice_cube_counter++;

  ic.top_left_x = 207;
  ic.top_left_y = 151;
  ic.bot_right_x = 207 + 30;
  ic.bot_right_y = 151 + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, 207, 151);
  ice_cube_counter++;

  // PATH CUBES
  // playerPosition.x = 207;
  // playerPosition.y = 271;
  x_ = 207;
  y_ = 271;

  for (int y = 0; y < 3; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ -= 30;
  }

  x_ = 177;
  y_ = 181;

  for (int x = 0; x < 14; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 177;
  y_ = 121;

  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 507;
  y_ = 121;

  for (int x = 0; x < 3; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  draw_sprite(path_cube, 177, 151);
  path_counter++;
  draw_sprite(path_cube, 507, 151);
  path_counter++;
  draw_sprite(path_cube, 237, 151);
  path_counter++;
  draw_sprite(path_cube, 567, 151);
  path_counter++;

  x_ = 327;
  y_ = 151;

  for (int x = 0; x < 4; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  draw_sprite(path_cube, 357, 211);
  path_counter++;
  draw_sprite(path_cube, 387, 211);
  path_counter++;

  x_ = 327;
  y_ = 271;
  for (int x = 0; x < 4; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    draw_sprite(path_cube, x_, y_ - 30);
    path_counter++;
    x_ += 30;
  }

  draw_sprite(path_cube, 537, 271);
  path_counter++;
  draw_sprite(path_cube, 537, 241);
  path_counter++;
  draw_sprite(path_cube, 537, 211);
  path_counter++;

  // cubo vermelho
  x_ = 537;
  y_ = 271;
  finish_level4.top_left_x = 537;
  finish_level4.top_left_y = 271;
  finish_level4.bot_right_x = 537 + 30;
  finish_level4.bot_right_y = 271 + 30;
  draw_sprite(red_cube, x_, y_);
  path_counter--;

  // player
  draw_sprite(player, playerPosition.x, playerPosition.y);

  ice_melt();

  // check if player has completed the path
  if (playerPosition.x == finish_level4.top_left_x && playerPosition.y == finish_level4.top_left_y) {
    if (check_all_ice_melted()) {
      gameLevel = LEVEL_5;
    }
    else {
      return;
    }
  }

  level4_draw_counter++;
  check_fell_into_water();
}
