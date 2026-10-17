/**
 * @file game.h
 * @brief Core game state manager, rendering loop, and input handling.
 */

#ifndef __GAME_H
#define __GAME_H

#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

/**
 * @brief Enum defining all possible states of the game.
 */
typedef enum {
    MAIN_MENU,
    NAME_INPUT,
    PLAYING,
    LEVEL_COMPLETE,
    LEVEL_SELECT,
    GAME_OVER,
    SCOREBOARD
} GameState;

/**
 * @brief Initializes the game components, variables, and systems.
 */
void game_init();

/**
 * @brief Initializes a specific level and its configuration.
 * * @param level_number The ID of the level to start.
 */
void game_init_level(int level_number);

/**
 * @brief Core update function managing game's logic step.
 */
void game_update();

/**
 * @brief Core render function that draws the current state to the screen.
 */
void game_render();

/**
 * @brief Checks if the game loop should terminate.
 * * @return true if the game should close, false otherwise.
 */
bool game_should_quit();

/**
 * @brief Distributes keyboard scancodes to the current game state.
 * * @param scancode The input scancode.
 */
void game_on_key(uint8_t scancode);

/**
 * @brief Distributes mouse packets and positions to the current game state.
 * * @param pp Pointer to the mouse packet.
 */
void game_on_mouse(struct packet *pp);

/**
 * @brief Transitions the game into a new state.
 * * @param new_state The target GameState.
 */
void game_change_state(GameState new_state);

/**
 * @brief Sets the flag to terminate the main loop.
 */
void game_set_quit();

/**
 * @brief Renders the level selection screen.
 * * @param mouse_x Current mouse x-coordinate.
 * @param mouse_y Current mouse y-coordinate.
 */
void level_select_render(int mouse_x, int mouse_y);

#endif
