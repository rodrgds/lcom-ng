#ifndef _LCOM_MENUS_H_
#define _LCOM_MENUS_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "configs.h"
#include "controller/graphics/graphics.h"
#include "controller/mouse/mouse.h"
#include "model/sprite.h"
#include "model/model.h"
#include "game/game.h"
#include "model/player.h"
#include "controller/rtc/rtc.h"

/**
 * @brief Function that draws the next frame on the screen, taking into account the current state of the game.
*/
void (draw_next_frame)();

/**
 * @brief Function frees the buffer used to draw the initial menu.
*/
void (free_menu_buffer)();

/**
 * @brief Function that draws the initial menu on the screen, which allows the player to choose between starting the game or seeing the controls.
*/
void (draw_initial_menu)();

/**
 * @brief Function that frees the buffer used to draw the controls menu.
*/
void (free_controls_menu_buffer)();

/**
 * @brief Function that draws the controls menu on the screen, which explains the controls of the game.
*/
void (draw_controls_menu)();

/**
 * @brief Function that draws the game on the screen, according to the current state of the game, including the players, the arena and the scoreboard.
*/
void (draw_game)();

/**
 * @brief Function that draws the scoreboard on the screen of the game, according to the current score, game time and territory of each player.
*/
int (draw_scoreboard)();

/**
 * @brief Function that draws the current date on the screen.
*/
int (draw_time)();

/**
 * @brief Function that draws the numbers on the screen, according to the current time.
 * @param num Sprite of the number to draw.
 * @param x X position to draw the number.
 * @param y Y position to draw the number.
 * @return 0 if the number was successfully drawn, 1 otherwise.
*/
int (draw_numbers)(Sprite *num, int x, int y);

/**
 * @brief Function that frees the buffer used to draw the end menu.
*/
void (free_end_menu_buffer)();

/**
 * @brief Function that draws in the end menu the final score of the winner.
 * @param winner Percentage of the territory of the winner.
*/
void (draw_endmenu_score)(int winner);

/**
 * @brief Function that draws the end menu on the screen, according to the game winner. If the game ended in a draw, it will return to the initial menu.
*/
void (draw_end_menu)();

/**
 * @brief Function that draws the mouse on the screen.
*/
void (draw_mouse)();

/**
 * @brief Function that draws the arena on the screen.
 * @param sprite Sprite to draw.
 * @return 0 if the arena was successfully drawn, 1 otherwise.
*/
int (draw_arena)(Sprite *sprite);

/**
 * @brief Function that draws a sprite on the screen.
 * @param sprite Sprite to draw.
 * @param x X position to draw the sprite.
 * @param y Y position to draw the sprite.
 * @param buffer Buffer to draw the sprite.
 * @return 0 if the sprite was successfully drawn, 1 otherwise.
*/
int (draw_sprite_xpm)(Sprite *sprite, int x, int y, uint8_t *buffer);

/**
 * @brief Function that chooses the frame buffer to use, acording to the double buffering variable.
 * @param mode Graphics mode to use.
 * @return 0 if the frame buffer was successfully chosen, 1 otherwise.
*/
int (choose_frame_buffer)(uint16_t mode);

/**
 * @brief Function that syncs the main buffer with the secondary buffer.
*/
void (sync_to_main_buffer)();

#endif
