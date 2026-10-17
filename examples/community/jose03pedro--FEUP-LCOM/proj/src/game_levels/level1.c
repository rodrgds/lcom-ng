/**
 * @file level1.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "level1.h"

int level1_draw_counter = 0;
extern PlayerPosition playerPosition;
extern PlayerPosition copy;
Box finish_level1;
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
extern Sprite_t *level1_tag;

void update_player_starting_position1() {
  playerPosition.x = 537;
  playerPosition.y = 361;
  copy.x = 537;
  copy.y = 361;
}

void draw_game_level1() {
  memset(&ice_cubes, 0, sizeof(ice_cubes));
  path_counter = 0;

  if (level1_draw_counter == 0) {
    memset(&water_path, 0, sizeof(water_path));
    update_player_starting_position1();
    water_counter = 0;
  }

  // draw level1 tag on the top left corner
  draw_sprite(level1_tag, 135, 32);

  // draw level1 ice cubes
  int x_ = 147;
  int y_ = 331;
  int ice_cube_counter = 0;
  Box ic;
  for (int x = 0; x < 15; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }
  ic.top_left_x = 147;
  ic.top_left_y = 361;
  ic.bot_right_x = 147 + 30;
  ic.bot_right_y = 361 + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, 147, 361);
  ice_cube_counter++;

  ic.top_left_x = 567;
  ic.top_left_y = 361;
  ic.bot_right_x = 567 + 30;
  ic.bot_right_y = 361 + 30;
  ice_cubes[ice_cube_counter] = ic;
  draw_sprite(ice_cube, 567, 361);
  ice_cube_counter++;

  x_ = 147;
  y_ = 391;
  for (int x = 0; x < 15; x++) {
    ic.top_left_x = x_;
    ic.top_left_y = y_;
    ic.bot_right_x = x_ + 30;
    ic.bot_right_y = y_ + 30;
    ice_cubes[ice_cube_counter] = ic;
    draw_sprite(ice_cube, x_, y_);
    ice_cube_counter++;
    x_ += 30;
  }

  // draw level1 path cubes
  x_ = 177;
  y_ = 361;
  for (int x = 0; x < 13; x++) {
    draw_sprite(path_cube, x_, y_);
    path_counter++;
    x_ += 30;
  }

  // draw level1 red cube
  x_ = 177;
  y_ = 361;
  finish_level1.top_left_x = 177;
  finish_level1.top_left_y = 361;
  finish_level1.bot_right_x = 177 + 30;
  finish_level1.bot_right_y = 361 + 30;
  draw_sprite(red_cube, x_, y_);
  path_counter--;

  // draw player
  draw_sprite(player, playerPosition.x, playerPosition.y);

  ice_melt();

  // check if player has completed the path
  if (playerPosition.x == finish_level1.top_left_x && playerPosition.y == finish_level1.top_left_y) {
    if (check_all_ice_melted()) {
      gameLevel = LEVEL_2;
    }
    else {
      return;
    }
  }

  level1_draw_counter++;
  check_fell_into_water();
}

void update_player_starting_position2() {
  playerPosition.x = 177;
  playerPosition.y = 361;
  copy.x = 177;
  copy.y = 361;
}
