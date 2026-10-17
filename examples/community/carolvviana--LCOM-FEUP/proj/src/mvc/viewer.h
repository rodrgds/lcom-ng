#pragma once

#include <lcom/lcf.h>

#include "gpu/gpu.h"
#include "xpms.h"
#include "model.h"
#include "../rtc/rtc.h"

struct Ball;
struct Player;

/**
* @brief Draws a xpm menu image on the screen
*/
void (draw_menu)();

/**
* @brief Draws a xpm mouse image on the screen
*/
void (draw_mouse)();

/**
* @brief Draws a xpm arena image on the screen
*/
void (draw_arena)();

/**
* @brief Draws a xpm arena image on the screen for multiplayer mode
*/
void (draw_arena_multiplayer)();

/**
* @brief Draws a xpm "credits/instructions" menu image on the screen
*/
void (draw_credits)();

/**
* @brief Draws a background on the screen
*/
void (draw_background)(uint32_t *color);

/**
* @brief Draws a player on the screen
*/
void (draw_player)(struct Player *player);

/**
* @brief Draws a ball on the screen
*/
void (draw_ball)(struct Ball *ball);

/**
* @brief Draws the scores on the screen
*/
void (draw_scores)();

/**
* @brief Draws the time on the screen
*/
void (draw_time)();

/**
* @brief Draws the "blocked" menu on the screen
*/
void (draw_blocked)();

/**
* @brief Auxiliar to get the image of the number
* @param number Number of index to get the image
*/
int (get_number_image)(int number);

/**
* @brief Auxiliar to get the image of the score
*
* @param score the score image index to be drawn
*/
int (getScoreImage)(int score);

/**
* @brief Draws the game over menu on the screen
*/
void (draw_game_over)();

/**
* @brief Draws the game win menu on the screen
*/
void (draw_game_win)();
