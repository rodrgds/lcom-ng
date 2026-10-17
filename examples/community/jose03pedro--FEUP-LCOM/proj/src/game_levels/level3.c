/**
 * @file level3.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "level3.h"

int level3_draw_counter = 0;
extern PlayerPosition playerPosition;
extern PlayerPosition copy;
Box finish_level3;
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
extern Sprite_t *level3_tag;

void update_player_starting_position3() {
  playerPosition.x = 537;
  playerPosition.y = 271;
  copy.x = 537;
  copy.y = 271;
}

void draw_game_level3() {
  memset(&ice_cubes, 0, sizeof(ice_cubes));
  path_counter = 0;

  if (level3_draw_counter == 0) {
    memset(&water_path, 0, sizeof(water_path));
    update_player_starting_position3();
    water_counter = 0;
  }

  // draw level3 ice cubes
  int x_ = 177;
  int y_ = 271;
  int ice_cube_counter = 0;
  Box ic;
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

  x_ = 177;
  y_ = 421;
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

  x_ = 357;
  y_ = 421;
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

  x_ = 447;
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

  x_ = 567;
  y_ = 271;
  for (int y = 0; y < 6; y++) {
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
  y_ = 241;
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

  x_ = 507;
  y_ = 271;
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

  x_ = 477;
  y_ = 331;
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
  ic.top_left_x = x_;
  ic.top_left_y = y_;
  ic.bot_right_x = x_ + 30;
  ic.bot_right_y = y_ + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, x_, y_);
  ice_cube_counter++;

  x_ = 237;
  y_ = 241;
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

  x_ = 267;
  y_ = 331;
  for (int x = 0; x < 2; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ -= 30;
  }

  // draw level3 tag on the top left corner
  draw_sprite(level3_tag, 135, 32);

  // draw level3 path cubes
  x_ = 537;
  y_ = 271;
  for (int y = 0; y < 5; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 507;
  y_ = 361;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 447;
  y_ = 361;
  for (int x = 0; x < 2; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  x_ = 417;
  y_ = 361;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 387;
  y_ = 361;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 237;
  y_ = 361;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 207;
  y_ = 271;
  for (int y = 0; y < 5; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 297;
  y_ = 331;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 327;
  y_ = 331;
  for (int y = 0; y < 2; y++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    y_ += 30;
  }

  x_ = 267;
  y_ = 361;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  x_ = 357;
  y_ = 361;
  draw_sprite(path_cube, x_, y_);
  path_counter++;

  // draw red cube
  x_ = 207;
  y_ = 271;
  finish_level3.top_left_x = 207;
  finish_level3.top_left_y = 271;
  finish_level3.bot_right_x = 207 + 30;
  finish_level3.bot_right_y = 271 + 30;
  draw_sprite(red_cube, x_, y_);
  path_counter--;

  // draw player
  draw_sprite(player, playerPosition.x, playerPosition.y);

  ice_melt();

  // check if player has completed the path
  if (playerPosition.x == finish_level3.top_left_x && playerPosition.y == finish_level3.top_left_y) {
    if (check_all_ice_melted()) {
      gameLevel = LEVEL_4;
    }
    else {
      return;
    }
  }

  level3_draw_counter++;
  check_fell_into_water();
}
