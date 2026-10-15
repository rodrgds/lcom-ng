#ifndef _GAME_VIEWER_H_
#define _GAME_VIEWER_H_

/**
 * @file game_viewer.h
 * @brief Header file containing function declarations for game viewer functions.
 */

#include "viewer.h"
#include "controller/timer/timer.h"

/** @defgroup game_viewer game_viewer
 * @{
  */
extern Sprite* map;
extern Sprite* lava;
extern Sprite* map;
extern Sprite* background;
extern Sprite* little_block;
extern Sprite* big_block;
extern Sprite* little_plank;
extern Sprite* big_plank;
extern Sprite* home;
extern Sprite* bamboo;

/**
 * @brief Draws the game menu including the map, inventory, background, timer, home, and other sprites.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_game_menu)();
/**
 * @brief Draws the inventory containers.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_inventory)();
/**
 * @brief Draws a container at the specified position and with the given width.
 * @param x The x-coordinate of the container.
 * @param y The y-coordinate of the container.
 * @param width The width of the container.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_container)(uint16_t x, uint16_t y, uint16_t width);
/**
 * @brief Draws the map borders.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_map)();
/**
 * @brief Draws the timer in the game menu.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_timer)();
/**
 * @brief Draws the game cursor based on the item state.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_game_cursor)();
#endif
