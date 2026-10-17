#ifndef PACMAN_MOVEMENT_CONTROLLER_H
#define PACMAN_MOVEMENT_CONTROLLER_H

#define PACMAN_STEP 10
#define SCORE 10

/**
 * @brief Changes pacman's position to the position above it.
 * 
 */
void (pacmanMoveUp)();

/**
 * @brief Changes pacman's position to the position bellow it.
 * 
 */
void (pacmanMoveDown)();

/**
 * @brief Changes pacman's position to its left position.
 * 
 */
void (pacmanMoveLeft)();

/**
 * @brief Changes pacman's position to its right position.
 * 
 */
void (pacmanMoveRight)();

/**
 * @brief Check if the pacman collides with any game element and if so, updates the game
 * state/time/score according to the element type.
 * 
 */
void (pacmanCollisionHandler)();

#endif
