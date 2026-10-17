#ifndef GHOSTS_MOVEMENT_CONTROLLER_H
#define GHOSTS_MOVEMENT_CONTROLLER_H

#define GHOST_STEP 5

/**
 * @brief Changes ghost's position to the position above it.
 * 
 * 
 */
void (moveUp)(int ghostIndex);

/**
 * @brief Changes ghost's position to the position bellow it.
 * 
 */
void (moveDown)(int ghostIndex);

/**
 * @brief Changes ghost's position to its left position.
 * 
 */
void (moveLeft)(int ghostIndex);

/**
 * @brief Changes ghost's position to its right position.
 * 
 */
void (moveRight)(int ghostIndex);

/**
 * @brief Changes position of all ghosts.
 * 
 */
void (move_ghosts)();

#endif
