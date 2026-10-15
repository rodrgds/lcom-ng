#ifndef HUD_CONTROLLER_H
#define HUD_CONTROLLER_H

#include <stdbool.h>
#include "../game.h"

/**
 * @brief Handles mouse clicks on the HUD.
 *
 * Checks if the click coordinates intersect with any interactive HUD elements
 * (like the hotbar) and applies the corresponding logic (e.g. equipping a weapon).
 *
 * @param context The current game context.
 * @param x The mouse X coordinate.
 * @param y The mouse Y coordinate.
 * @return true if the click was consumed by the HUD, false otherwise.
 */
bool hud_controller_handle_click(GameContext *context, int x, int y);

#endif /* HUD_CONTROLLER_H */
