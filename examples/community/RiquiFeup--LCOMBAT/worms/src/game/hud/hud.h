#ifndef HUD_H
#define HUD_H

#include "game.h"

/**
 * @brief Draws the full in-game HUD overlay.
 *        Call after all scene drawing, before renderer_flip().
 *
 * Reads worm health, active player, ticks_left, and weapon from context.
 * Does not modify context.
 *
 * @param context Game state to read from. NULL is a no-op.
 */
void hud_draw(const GameContext *context);

/**
 * @brief Gets the weapon type assigned to a hotbar slot.
 * @param slot_index The zero-based index of the slot.
 * @return The weapon type, or WEAPON_NONE if the index is out of bounds.
 */
WeaponType hud_get_hotbar_weapon(int slot_index);

#endif /* HUD_H */
