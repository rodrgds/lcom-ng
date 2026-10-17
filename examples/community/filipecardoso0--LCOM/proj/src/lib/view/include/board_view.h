#ifndef BOARD_VIEW_H
#define BOARD_VIEW_H

#include "../../model/include/board_model.h"
#include "wall_view.h"
#include "snake_view.h"
#include "fruit_view.h"

/**
 * @brief Draws the board game in its initial state, that is, it will draw every element on the screen, with the help of the drawing methods of each element.
 * 
 * @param board Structure that contains the details of all elements in the game.
 * @return 0 if it succeded, otherwise it returns 1.
 */
int board_first_draw(board_t* board);

/**
 * @brief Redraws the elements of the game while they update their position.
 * 
 * @param board Structure that contains the details of all elements in the game.
 * @return 0 if it succeded, otherwise it returns 1.
 */
int board_draw(board_t* board);

#endif
