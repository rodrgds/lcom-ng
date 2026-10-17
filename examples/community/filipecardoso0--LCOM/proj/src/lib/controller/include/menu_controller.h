#ifndef _MENU_CONTROLLER_H_
#define _MENU_CONTROLLER_H_

#include "../../model/include/menu_model.h"
#include "../../state/include/state.h"

/**
 * @brief Updates the specified menu's state according to the input action.
 * 
 * @param menu Points to the menu to be updated.
 * @param action Input action.
 */
int menu_step(menu_t* menu, action_t action); 

#endif

