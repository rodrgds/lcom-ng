#ifndef DRAW_H
#define DRAW_H

#include <math.h>
#include "our_framework/framework_essencials.h"
#include "bullet.h"
#include "xpm/tower_orange/tower_orange.h"
#include "xpm/tower_purple/tower_purple.h"
#include "xpm/tower_blue/tower_blue.h"
#include "xpm/cursor.xpm"
#include "xpm/background_without_towers.xpm"
#include "xpm/menu.xpm"
#include "xpm/start_game.xpm"
#include "xpm/quit.xpm"
#include "xpm/start_game_glow.xpm"
#include "xpm/quit_glow.xpm"
#include "xpm/numbers/numbers.h"
#include "xpm/enemy_fire_skull/enemy_fire_skull_walking.h"
#include "xpm/game_over.xpm"

xpm_image_t cursor_img;
uint8_t * cursor_pixmap;

xpm_image_t background_img;
uint8_t * background_pixmap;

xpm_image_t game_over_img;
uint8_t * game_over_pixmap;

xpm_image_t menu_img;
uint8_t * menu_pixmap;

xpm_image_t start_game_img;
uint8_t * start_game_pixmap;

xpm_image_t quit_img;
uint8_t * quit_pixmap;

xpm_image_t start_game_glow_img;
uint8_t * start_game_glow_pixmap;

xpm_image_t quit_glow_img;
uint8_t * quit_glow_pixmap;

int last_position_orange[6];
int last_position_purple[6];
int last_position_blue[6];

/**
 * @brief loads xpms
 * 
 */
void load_xpms();

/**
 * @brief function that draws the lives
 * 
 * @param lives number of lives
 */
void draw_lives(int lives);

/**
 * @brief function that draws the money
 * 
 * @param coins number of coins
 */
void draw_money(int coins);

/**
 * @brief function that draws a specific number
 * 
 * @param number number to draw
 * @param x number x position
 * @param y number y position
 */
void draw_number(int number, uint16_t x, uint16_t y);

/**
 * @brief function that draws orange towers in the corresponding rotating angle
 * 
 * @param tower_id tower id
 * @param tower_x tower x position
 * @param tower_y tower y position
 */
void draw_orange_tower_directing_to_enemy(int tower_id, int tower_x, int tower_y, enemy* enemies, int range);

/**
 * @brief function that draws purple towers in the corresponding rotating angle
 * 
 * @param tower_id tower id
 * @param tower_x tower x position
 * @param tower_y tower y position
 */
void draw_purple_tower_directing_to_enemy(int tower_id, int tower_x, int tower_y, enemy* enemies, int range);

/**
 * @brief function that draws blue towers in the corresponding rotating angle
 * 
 * @param tower_id tower id
 * @param tower_x tower x position
 * @param tower_y tower y position
 */
void draw_blue_tower_directing_to_enemy(int tower_id, int tower_x, int tower_y, enemy* enemies, int range);

#endif
