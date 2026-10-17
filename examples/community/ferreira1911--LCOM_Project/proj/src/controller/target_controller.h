#ifndef TARGET_CONTROLLER_H
#define TARGET_CONTROLLER_H

#include <lcom/lcf.h>

#include "model/target.h"

#define MAX_ACTIVE_TARGETS 3 /**< @brief Maximum number of active targets on the screen */
#define TARGET_WIDTH 75 /**< @brief Width of the target */
#define TARGET_HEIGHT 60 /**< @brief Height of the target */

/**
 * @brief Clears all targets (sets them to invisible and resets their data).
 */
void (target_controller_clear_targets)();

/**
 * @brief Initializes targets for mode 1.
 */
void (target_controller_init_mode1)();

/**
 * @brief Initializes targets for mode 2 (falling from the top).
 */
void (target_controller_init_mode2)();

/**
 * @brief Initializes targets for mode 3 (horizontal movement with falling).
 */
void (target_controller_init_mode3)();

/**
 * @brief Updates the state of targets in mode 1 (respawn after hit or timeout).
 */
void (target_controller_update_mode1)();

/**
 * @brief Updates the state of targets in mode 2 (falling vertically).
 */
void (target_controller_update_mode2)();

/**
 * @brief Updates the state of targets in mode 3 (horizontal + vertical movement).
 */
void (target_controller_update_mode3)();

/**
 * @brief Updates horizontal and vertical movement of targets.
 */
void (target_controller_horizontal_update)();

/**
 * @brief Updates the falling motion of targets.
 */
void (target_controller_fall_update)();

/**
 * @brief Draws all visible targets on the screen.
 */
void (target_controller_draw)();

/**
 * @brief Checks if a given coordinate (x, y) hits any target.
 * 
 * @param x The x coordinate of the click.
 * @param y The y coordinate of the click.
 * @return true if a target was hit, false otherwise.
 */
bool (target_controller_check_hit)(uint16_t x, uint16_t y);

/**
 * @brief Checks if a given coordinate (x, y) hits the target in mode 3.
 * 
 * @param x The x coordinate of the click.
 * @param y The y coordinate of the click.
 * @return true if the target was hit, false otherwise.
 */
bool target_controller_check_hit_mode3(uint16_t x, uint16_t y);

/**
 * @brief Generates a random position inside the game area (used in mode 1).
 * 
 * @param x Pointer to store the generated x coordinate.
 * @param y Pointer to store the generated y coordinate.
 */
void generate_random_position_in_game_area(int16_t *x, int16_t *y);

/**
 * @brief Generates a random top position for falling targets (mode 2).
 * 
 * @param x Pointer to store the generated x coordinate.
 * @param y Pointer to store the generated y coordinate.
 */
void generate_random_position_in_game_area_mode2(int16_t *x, int16_t *y);

/**
 * @brief Generates a random position and direction for targets in mode 3.
 * 
 * @param x Pointer to store the generated x coordinate.
 * @param y Pointer to store the generated y coordinate.
 * @param direction Pointer to store the movement direction (1: right, -1: left).
 */
void generate_random_position_in_game_area_mode3(int16_t *x, int16_t *y, int *direction);

/**
 * @brief Checks if a new target at position (x, y) overlaps with an existing target.
 * 
 * @param a Pointer to the existing target.
 * @param x X coordinate of the new target.
 * @param y Y coordinate of the new target.
 * @param width Width of the new target.
 * @param height Height of the new target.
 * @return true if the new target overlaps the existing one, false otherwise.
 */
bool is_overlapping(Target *a, int16_t x, int16_t y, uint16_t width, uint16_t height);

#endif

