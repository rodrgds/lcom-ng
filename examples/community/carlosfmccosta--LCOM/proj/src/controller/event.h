/**
 * @file event.h
 * @brief Interface for handling input events from keyboard and mouse across different game states.
 *
 * This header defines the event structure and provides functions for creating and handling
 * keyboard and mouse events in various game states.
 */
#ifndef __EVENT_H
#define __EVENT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "KBC/mouse/mouse.h"
#include "KBC/keyboard/keyboard.h"
#include "game.h"
#include "view/menu.h"
#include "controller/state.h"
#include "view/write_name.h"
#include "view/leaderboard_page.h"

/**
 * 
 * @brief Handles input events from keyboard and mouse across different game states.
 */

/**
 * @enum event_type
 * @brief Defines the type of input event.
 */
typedef enum {
    EVENT_KEYBOARD, /**< Event from the keyboard */
    EVENT_MOUSE     /**< Event from the mouse */
} event_type;

/**
 * @struct event_t
 * @brief Represents an input event from the keyboard or mouse.
 */
typedef struct {
  event_type type;              /**< Type of event (keyboard or mouse) */
  struct packet * pack;         /**< Mouse packet if the event is from the mouse */
  struct key * k;               /**< Keyboard key if the event is from the keyboard */
  unsigned int prev_mouse_btn;  /**< Previous mouse button pressed: 0=none, 1=right, 2=left, 3=middle */
} event_t;

/**
 * @brief Creates a new mouse event.
 * 
 * @param pack Pointer to the mouse packet.
 * @param prev Previous mouse button state.
 * @return Pointer to the newly created mouse event.
 */
event_t *new_mouse_event(struct packet *pack, unsigned int prev);

/**
 * @brief Creates a new keyboard event.
 * 
 * @param k Pointer to the keyboard key structure.
 * @return Pointer to the newly created keyboard event.
 */
event_t *new_keyboard_event(struct key * k);

/**
 * @brief Creates an empty event with no data.
 * 
 * @return Pointer to the newly created empty event.
 */
event_t *create_empty_event();

/**
 * @brief Handles an input event during the game state.
 * 
 * @param e Pointer to the input event.
 * @param game Pointer to the game structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Always returns 0.
 */
int handle_event_game(event_t * e, game_t *game, sprite_t *cursor);

/**
 * @brief Handles a keyboard event during the game state.
 * 
 * @param e Pointer to the input event.
 * @param player Pointer to the player structure.
 * @param wall_arr Array of walls in the game.
 * @param enemies_arr Array of enemy pointers.
 * @return Always returns 0.
 */
int handle_keyboard_event_game(event_t * e, player_t * player, wall_array_t wall_arr, enemy_t ** enemies_arr);

/**
 * @brief Handles a mouse event during the game state.
 * 
 * @param e Pointer to the input event.
 * @param player Pointer to the player structure.
 * @param cursor Pointer to the cursor sprite.
 * @param bullets Array of bullet pointers.
 * @return Always returns 0.
 */
int handle_mouse_event_game(event_t * e, player_t * player, sprite_t * cursor, bullet_t* bullets[]);

/**
 * @brief Handles an input event during the menu state.
 * 
 * @param e Pointer to the input event.
 * @param menu Pointer to the menu structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Always returns 0.
 */
int handle_event_menu(event_t * e, menu_t *menu, sprite_t *cursor);

/**
 * @brief Handles a keyboard event during the menu state.
 * 
 * @param e Pointer to the input event.
 * @param menu Pointer to the menu structure.
 * @return Always returns 0.
 */
int handle_keyboard_event_menu(event_t * e, menu_t *menu);

/**
 * @brief Handles a mouse event during the menu state.
 * 
 * @param e Pointer to the input event.
 * @param menu Pointer to the menu structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Always returns 0.
 */
int handle_mouse_event_menu(event_t * e, menu_t *menu, sprite_t * cursor);

/**
 * @brief Handles an input event during the write-name screen.
 * 
 * @param e Pointer to the input event.
 * @param page Pointer to the write name page structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Returns 0 in success.
 */
int handle_event_page(event_t * e, write_name_t *page, sprite_t *cursor);

/**
 * @brief Handles a keyboard event during the write-name screen.
 * 
 * @param e Pointer to the input event.
 * @param page Pointer to the write name page structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Always returns 0.
 */
int handle_keyboard_event_page(event_t * e, write_name_t *page, sprite_t *cursor);

/**
 * @brief Handles a mouse event during the write-name screen.
 * 
 * @param e Pointer to the input event.
 * @param page Pointer to the write name page structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Always returns 0.
 */
int handle_mouse_event_page(event_t * e, write_name_t *page, sprite_t * cursor);

/**
 * @brief Handles an input event during the leaderboard screen.
 * 
 * @param e Pointer to the input event.
 * @param lb_page Pointer to the leaderboard page structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Always returns 0.
 */
int handle_event_lb(event_t *e, lb_page_t * lb_page, sprite_t *cursor);

/**
 * @brief Handles a keyboard event during the leaderboard screen.
 * 
 * @param e Pointer to the input event.
 * @param lb_page Pointer to the leaderboard page structure.
 * @return Always returns 0.
 */
int handle_keyboard_event_lb(event_t *e, lb_page_t *lb_page);

/**
 * @brief Handles a mouse event during the leaderboard screen.
 * 
 * @param e Pointer to the input event.
 * @param lb_page Pointer to the leaderboard page structure.
 * @param cursor Pointer to the cursor sprite.
 * @return Returns 0 in success and 1 if failure.
 */
int handle_mouse_event_lb(event_t *e, lb_page_t *lb_page, sprite_t *cursor);

#endif //__EVENT_H
