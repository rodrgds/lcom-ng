#ifndef FRUIT_CONTROLLER_H
#define FRUIT_CONTROLLER_H

#include "../../model/include/board_model.h"
#include "../../state/include/action.h"

/**
 * @brief Updates the specified fruit details in the board.
 * 
 * @param fruit Points to the fruit to be updated.
 * @param board Points to the board that contains the input fruit.
 */
void fruit_step(fruit_t* fruit, board_t* board);

#endif
