/**
 * @file ship.h
 * @brief Ship entity management.
 */
#ifndef SHIP_H
#define SHIP_H

#include "game.h"
#include "../input/input.h"
#include "../graphics/sprite.h"

/**
 * @brief Updates the ship's logic.
 * @param ship Pointer to the Ship entity.
 * @param input Pointer to the current InputState.
 */
void ship_update(Ship *ship, const InputState *input);

/**
 * @brief Draws the ship.
 * @param ship Pointer to the Ship entity.
 * @param sprite Pointer to the ship Sprite.
 * @param shield_active True if the shield is active.
 */
void ship_draw(const Ship *ship, const Sprite *sprite, bool shield_active);

#endif
