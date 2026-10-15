/**
 * @file view.h
 * @brief Header file containing drawing and rendering functions for the game interface and logic.
 */

#ifndef _VIEW_H_
#define _VIEW_H_

#include <lcom/lcf.h>
#include "graphics.h"
#include "../controller/mouse.h"
#include "../sprites/sprite.h"
#include <minix/syslib.h>
#include "../model/map.h"
#include "../model/menu.h"
#include "../rtc.h"
#include "../model/game.h"
#include "../model/gametypes.h"

/// Width of the game over screen
#define GAME_OVER_WIDTH 395

/// Height of the game over screen
#define GAME_OVER_HEIGHT 262

/// Width of the game win screen
#define GAME_WIN_WIDTH 395

/// Height of the game win screen
#define GAME_WIN_HEIGHT 262

/// General border spacing used in layout
#define BORDER_SPACING 20

/// Width of the timer border
#define TIMER_BORDER_WIDTH 190

// ===================== Initialization and Cleanup =====================

/**
 * @brief Initializes all game sprites.
 * 
 * Creates all sprite objects used in the game including menu elements,
 * game elements, and UI components.
 */
void (init_allsprites)();

/**
 * @brief Frees all allocated sprites.
 */
void (destroy_allsprites)();

// ===================== Cursor and Background Drawing =====================

/**
 * @brief Draws the mouse cursor at the specified coordinates.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void (draw_cursor)(int x, int y);

/**
 * @brief Draws the main menu background.
 */
void (draw_menu_background)();

/**
 * @brief Draws the background for the game view.
 */
void (draw_game_background)();

/**
 * @brief Draws the appropriate background based on context (menu or game).
 */
void (draw_background)();

/**
 * @brief Draws a specific pixel from the background at a given coordinate.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void (draw_background_pixel)(int x, int y);

// ===================== Cell Drawing (Game) =====================

/**
 * @brief Draws a closed cell.
 */
void (draw_closedCell)(int x, int y);

/**
 * @brief Draws an opened cell.
 */
void (draw_openCell)(int x, int y);

/**
 * @brief Draws a cell with a flag.
 */
void (draw_flaggedCell)(int x, int y);

/**
 * @brief Draws a cell containing a bomb.
 */
void (draw_bombCell)(int x, int y);

/**
 * @brief Draws the exploded bomb cell.
 */
void (draw_explodedCell)(int x, int y);

/**
 * @brief Draws a cell with the number 1 (indicating 1 adjacent bomb).
 */
void (draw_cell1)(int x, int y);

/**
 * @brief Draws a cell with the number 2.
 */
void (draw_cell2)(int x, int y);

/**
 * @brief Draws a cell with the number 3.
 */
void (draw_cell3)(int x, int y);

/**
 * @brief Draws a cell with the number 4.
 */
void (draw_cell4)(int x, int y);

/**
 * @brief Draws a cell with the number 5.
 */
void (draw_cell5)(int x, int y);

/**
 * @brief Draws a cell with the number 6.
 */
void (draw_cell6)(int x, int y);

/**
 * @brief Draws a cell with the number 7.
 */
void (draw_cell7)(int x, int y);

/**
 * @brief Draws a cell with the number 8.
 */
void (draw_cell8)(int x, int y);

/**
 * @brief Draws a number at a specific location using sprites.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param number Number to draw.
 */
void (draw_number)(int x, int y, unsigned int number);

/**
 * @brief Draws a bomb at a given location.
 */
void (draw_bomb)(int x, int y);

/**
 * @brief Draws the lamp icon indicating safe cells.
 */
void (draw_lamp)(Map *map);

/**
 * @brief Draws the number of bombs remaining based on the map.
 */
void (draw_number_of_bombs)(Map *map);

/**
 * @brief Draws the game over screen.
 */
void (draw_game_over)(int x, int y);

/**
 * @brief Draws the game win screen.
 */
void (draw_game_win)(int x, int y);

/**
 * @brief Draws the entire game map.
 */
void (draw_map)(Map *map);

/**
 * @brief Draws all game elements (map, timer, etc).
 */
void (draw_game)();

/**
 * @brief Draws the current time on screen.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param seconds Time in seconds.
 */
void (draw_time)(int x, int y, int seconds);

/**
 * @brief Draws the colon (:) separator for the time display.
 */
void (draw_colon)(int x, int y);

/**
 * @brief Draws the final win screen, including time and menu options.
 */
void (draw_game_win_screen)(int elapsed_seconds, int gw_x, int gw_y, int timer_x, int timer_y, int mouse_x, int mouse_y);

/**
 * @brief Draws a large decorative border.
 */
void (draw_big_border)(int x, int y);

/**
 * @brief Draws a medium-sized border.
 */
void (draw_medium_border)(int x, int y);

/**
 * @brief Draws a small border.
 */
void (draw_small_border)(int x, int y);

/**
 * @brief Draws a border specifically for the timer display.
 */
void (draw_timer_border)(int x, int y);

/**
 * @brief Draws the "leave game" button.
 */
void (draw_leave)(int x, int y);

// ===================== Menu Drawing =====================

/**
 * @brief Draws the menu interface.
 */
void (draw_menu)(Menu *menu);

/**
 * @brief Draws a menu button with an associated label.
 */
void (draw_button)(Button *btn, Sprite *label_sprite);

/**
 * @brief Draws the left arrow, changing appearance if hovered.
 */
void (draw_left_arrow)(int x, int y, bool hovered);

/**
 * @brief Draws the right arrow, changing appearance if hovered.
 */
void (draw_right_arrow)(int x, int y, bool hovered);

/**
 * @brief Draws the "Start" button.
 */
void (draw_start_button)(Menu *menu);

/**
 * @brief Draws the "Difficulty" button.
 */
void (draw_difficulty_button)(Menu *menu);

/**
 * @brief Draws the "Mode" button.
 */
void (draw_mode_button)(Menu *menu);

/**
 * @brief Draws the "Exit" button.
 */
void (draw_exit_button)(Menu *menu);

#endif /* _VIEW_H_ */

