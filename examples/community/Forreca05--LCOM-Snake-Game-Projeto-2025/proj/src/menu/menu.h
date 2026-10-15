#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "../controllers/gpu/gpu.h"
#include "../controllers/gpu/sprites/sprite.h"
#include "../controllers/gpu/sprites/dynamicSprite.h"
#include "../controllers/KBC/KBC.h"
#include "../controllers/KBC/i8042.h"
#include "../controllers/KBC/keyboard/keyboard.h"
#include "../controllers/KBC/mouse/mouse.h"
#include "../game/UTILS.h"
#include "../input/input.h"

/**
 * @brief Runs the main menu of the application.
 *
 * This function displays and manages the main menu, allowing the user to select
 * between different game modes or options. It updates the value pointed to by
 * @p isMultiplayer to indicate whether the multiplayer mode was selected.
 *
 * @param[out] isMultiplayer Pointer to a boolean variable that will be set to true if
 *                           multiplayer mode is selected, false otherwise.
 * @return int Returns 0 on success, or a non-zero error code on failure.
 */
int run_menu(bool* isMultiplayer);

#endif
