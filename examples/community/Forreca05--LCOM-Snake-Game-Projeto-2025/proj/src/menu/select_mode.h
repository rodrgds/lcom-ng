#ifndef SELECT_MODE_H
#define SELECT_MODE_H

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "choose_input_method.h"
#include "../controllers/gpu/gpu.h"
#include "../controllers/gpu/sprites/sprite.h"
#include "../controllers/KBC/KBC.h"
#include "../controllers/KBC/i8042.h"
#include "../controllers/KBC/keyboard/keyboard.h"
#include "../controllers/KBC/mouse/mouse.h"
#include "../game/elements/snake.h"
#include "../game/elements/freesnake.h"
#include "../game/game.h"
#include "../game/UTILS.h"
#include "../input/input.h"

/**
 * @brief Runs the mode selection menu.
 *
 * Displays the mode selection menu to the user and updates the provided flag
 * to indicate whether the classic mode was selected.
 *
 * @param[out] isClassic Pointer to a boolean variable that will be set to true if classic mode is selected, false otherwise.
 * @return int Returns 0 on success, or a negative value if an error occurs.
 */
int run_select_mode(bool* isClassic);

#endif
