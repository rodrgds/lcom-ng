/**
 * @file menu.h
 * @brief Menu state logic and rendering.
 */
#ifndef MENU_H
#define MENU_H

#include "game.h"
#include "../input/input.h"
#include "../graphics/ui.h"

/**
 * @brief Initializes the menu system.
 */
void menu_init(void);

/**
 * @brief Updates the menu logic.
 * @param game Pointer to the Game state.
 * @param input Pointer to the current InputState.
 */
void menu_update(Game *game, const InputState *input);

/**
 * @brief Draws the menu screen.
 * @param game Pointer to the Game state.
 */
void menu_draw(Game *game);

#endif
