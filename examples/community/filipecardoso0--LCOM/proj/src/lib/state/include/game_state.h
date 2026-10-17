#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include "../../controller/include/board_controller.h"
#include "../../view/include/board_view.h"
#include "state.h"


/**
 * @brief Changes the game state according to the action made by the user.
 * 
 * @param action input action: it can be whether is uses an valid game key, selects an option or when he/she wants to exit or does nothing.
 */
void game_state_step(action_t action);

#endif 
