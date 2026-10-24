/**
 * @file background.h
 * @brief Background rendering and animation logic.
 */
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "game.h"

/**
 * @brief Initializes background resources (e.g. sprites and caches).
 */
void background_init(void);

/**
 * @brief Destroys background resources.
 */
void background_destroy(void);

/**
 * @brief Draws the animated background.
 * @param game Pointer to the game state.
 */
void background_draw(const Game *game);

#endif
