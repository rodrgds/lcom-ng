#ifndef __PROJ_VIEW_H
#define __PROJ_VIEW_H

#include <math.h>

#include "controller/ih/ih.h"
#include "controller/kbc/mouse.h"
#include "model/game/game.h"

/** @defgroup view view
 * @{
 *
 * Functions for rendering the game and its menus
 */

/**
 * @brief Cleans up frame cache memory
 *
 * Frees the memory allocated for the frame cache and resets
 * the cache state. Should be called during game cleanup.
 */
void(cleanup_cache)(void);

/**
 * @brief Draws the start menu
 *
 * Renders the start menu to the screen.
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_start_menu)(Game *game);

/**
 * @brief Draws the pause menu
 *
 * Renders the pause menu to the screen.
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_pause_menu)(Game *game);

/**
 * @brief Draws the game
 *
 * Renders the game board, player, enemies, bricks, walls, and bombs
 * to the screen.
 *
 * @param game Pointer to the game to be drawn
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_game)(Game *game);

/**
 * @brief Draws the win screen
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_win_screen)(void);

/**
 * @brief Draws the lose screen
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_lose_screen)(void);

/**
 * @brief Draws the mouse cursor
 *
 * @param mouse_info Structure containing mouse position and button states
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_mouse)(mouse_info_t mouse_info);

/**
 * @brief Draws the next frame
 *
 * Determines the current game state and renders the appropriate
 * screen (start menu, pause menu, or game).
 *
 * @param game Pointer to the game instance
 */
void(draw_next_frame)(Game *game);

/**
 * @brief Draws text using the alphabet sprite sheet
 *
 * @param text The text string to draw (A-Z only)
 * @param x X-coordinate for the text
 * @param y Y-coordinate for the text
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_text)(const char *text, uint16_t x, uint16_t y);

/**
 * @brief Draws a number using the numbers sprite sheet
 *
 * @param number The number to draw
 * @param x X-coordinate for the number
 * @param y Y-coordinate for the number
 *
 * @return 0 upon success, non-zero otherwise
 */
int(draw_number)(int number, uint16_t x, uint16_t y);

/**@}*/

#endif /* __PROJ_VIEW_H */
