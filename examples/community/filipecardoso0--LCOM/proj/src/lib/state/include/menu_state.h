#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "../../controller/include/menu_controller.h"
#include "../../view/include/menu_view.h"
#include "state.h"


/**
 * @brief Changes the menu state according the action's state.
 * 
 * @param action action_t that contains the state of the action.
 */
void menu_state_step(action_t action);

#endif 
