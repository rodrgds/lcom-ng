#ifndef CHOOSE_INPUT_METHOD_H
#define CHOOSE_INPUT_METHOD_H

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "../controllers/gpu/gpu.h"
#include "../controllers/gpu/sprites/sprite.h"
#include "../controllers/gpu/sprites/dynamicSprite.h"
#include "../controllers/gpu/sprites/text.h"
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
 * @brief Runs the input method selection menu.
 *
 * Displays a menu to allow the user to choose an input method.
 *
 * @param[out] option Pointer to an integer where the selected option will be stored.
 * @param[in] isMultiplayer Boolean flag indicating if the game is in multiplayer mode.
 * @param[in] isClassic Boolean flag indicating if the game is in classic mode.
 * @return 0 on success, non-zero on failure.
 */
int run_choose_input_method(int* option, bool isMultiplayer, bool isClassic);

#endif
