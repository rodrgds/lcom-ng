/**
 * @file characters.h
 * @brief Defines structures and functions for player characters.
 * This file includes the definition of player character types,
 * an array to hold active player characters, and function prototypes
 * for initializing, creating, adding, updating, and drawing player units.
 */

#ifndef __CHARACTERS_H
#define __CHARACTERS_H

#include <lcom/lcf.h>
#include <lcom/xpm.h>

#include "enemy.h" 

#define MAX_PLAYER_CHARACTERS 15 /**Maximum number of player characters allowed on screen. */

#define PLAYER_CHAR_TYPE_1 0
#define PLAYER_CHAR_TYPE_2 1
#define PLAYER_CHAR_TYPE_3 2

extern Character *active_player_chars[MAX_PLAYER_CHARACTERS];
extern int num_active_player_chars;

/**
 * @brief Initializes player unit resources, such as loading sprites.
 * @return True if initialization is successful, false otherwise.
 */
bool init_player_units(void);

/**
 * @brief Creates a new player character unit.
 * @param pos The position (CharacterPos) where the unit will be created.
 * @param unit_type The type of player unit to create (e.g., PLAYER_CHAR_TYPE_1).
 * @return Pointer to the created Character, or NULL on failure.
 */
Character *create_player_unit(CharacterPos pos, int unit_type);

/**
 * @brief Adds a player unit to the game.
 * This function typically calls create_player_unit and then adds the unit to the active list.
 * @param pos The position (CharacterPos) where the unit will be spawned.
 * @param unit_type The type of player unit to add.
 */
void add_player_unit_to_game(CharacterPos pos, int unit_type);

/**
 * @brief Handles player input for spawning units.
 * @param scancode The scancode of the key pressed, used to determine which unit to spawn.
 * @note This function is declared but its implementation might be in a different file (e.g., input handler).
 */
void handle_player_unit_spawn_input(uint8_t scancode); 

/**
 * @brief Updates the state of all active player units.
 * This includes movement, AI logic (like attacking), and managing animations.
 * It does not handle direct spawning from input, but can manage timed or conditional spawning.
 */
void update_and_spawn_player_units(void); 

/**
 * @brief Draws all active player units to the screen.
 */
void draw_player_units(void);

/**
 * @brief Sets the timer frequency used for player character updates.
 * @param hz The frequency in Hertz.
 */
void set_timer_frequency_for_heroes(unsigned int hz);


#endif
