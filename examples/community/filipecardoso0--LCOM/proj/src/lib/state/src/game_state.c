#include "../include/game_state.h"

board_t* board = NULL;

void 
game_state_step(action_t action)
{

  if (action == EXIT) {
    board_delete(&board);
    set_app_state_menu();
    state_set_action(NOTHING);
    return;
  }

  if (board == NULL) {
    board = board_new(40, 30);
    board_first_draw(board);
  }
  
  if (board_step(board, action)) {
    board_delete(&board);
    state_set_action(NOTHING);
    set_app_state_menu();
    return;
  }
  board_draw(board);
}
