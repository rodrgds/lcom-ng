#ifndef BOARD_CONTROLLER_H
#define BOARD_CONTROLLER_H

#include "../../model/include/board_model.h"
#include "snake_controller.h"
#include "fruit_controller.h"

/**
 * @brief Updates the boards details according to the input action.
 * 
 * @param board Points to the board to be updated.
 * @param action Input action.
 */
int board_step(board_t* board, action_t action);

#endif
