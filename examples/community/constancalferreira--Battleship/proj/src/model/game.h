#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include "controller/timer.h"
#include "controller/keyboard.h"
#include "controller/mouse.h"
#include "controller/graphics.h"

#include "view/game_view.h"
#include "model/game_macro.h"

/**
 * @enum GameState
 * @brief Represents the various states the game can be in.
 */
typedef enum {
  MENU,       ///< Main menu (changes theme based on time of day)
  RULES,      ///< Rules screen (describes what Battleship is)
  HELP,       ///< Help screen (how to play and keyboard shortcuts)
  MODE,       ///< Mode selection screen
  START,      ///< Transition screen for changing players
  ARENA,      ///< Main gameplay arena
  GAME_OVER,  ///< Game over screen (when a player loses)
  EXIT        ///< Exit state (when the game closes)
} GameState;

/// @brief Latest keyboard scancode received
extern uint8_t scancode;

/// @brief Index for assembling mouse packets
extern uint8_t byte_index;

/// @brief Latest mouse packet received
extern struct packet mouse_packet;

/// @brief Current cursor x-coordinate
int cursor_x;

/// @brief Current cursor y-coordinate
int cursor_y;

/// @brief VBE mode information for the current graphics mode
extern vbe_mode_info_t mode_info;

/// @brief Pointer to the currently active frame buffer
extern uint8_t *current_buffer;

/// @brief Off-screen buffer for the arena view
extern uint8_t *arena_buffer;

/// @brief Off-screen buffer for the menu screen
extern uint8_t *menu_buffer;

/// @brief Off-screen buffer for the rules screen
extern uint8_t *rules_buffer;

/// @brief Off-screen buffer for the help screen
extern uint8_t *help_buffer;

/// @brief Off-screen buffer for the game over screen
extern uint8_t *game_over_buffer;

/// @brief Off-screen buffer for the start screen
extern uint8_t *start_buffer;

/// @brief Off-screen buffer for the game mode screen
extern uint8_t *mode_buffer;

/// @brief Current state of the game
extern GameState state;

/// @brief Previous state of the game (used for transitions)
extern GameState previous_state;

/**
 * @brief Changes the current game state.
 * 
 * @param new_state The new GameState to transition to.
 */
void set_state(GameState new_state);

/**
 * @brief Retrieves the current game state.
 * 
 * @return The current GameState.
 */
GameState get_state();

/**
 * @brief Retrieves the previous game state.
 * 
 * @return The GameState prior to the current one.
 */
GameState get_previous_state();

/**
 * @brief Timer interrupt handler for the game.
 * 
 * Called on every timer tick (e.g., to update animations or timers).
 */
void game_timer_handler();

/**
 * @brief Handles keyboard input based on the current game state.
 * 
 * Translates scancodes into actions like changing screens or quitting.
 */
void game_keyboard_handler();

/**
 * @brief Handles mouse input including movement and clicks.
 * 
 * Updates cursor position and enforces screen boundaries.
 */
void game_mouse_handler();

/**
 * @brief Frees all allocated graphics buffers.
 * 
 * Should be called during cleanup or state transitions.
 */
void free_buffers();

#endif // GAME_H
