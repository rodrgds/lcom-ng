#include "../include/board_controller.h"
#include "../../state/include/state.h"

int 
board_step(board_t* board, action_t action) 
{
  if (snake_step(board->snake1, board, action)) return 1;
  fruit_step(board->fruit, board);
  return 0;
}
