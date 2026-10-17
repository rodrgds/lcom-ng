#include <lcom/lcf.h>

#include "../include/fruit_controller.h"

void 
fruit_update_pos(fruit_t* fruit, board_t* board)
{
  position_t* position = NULL;
  bool colision = true;
  while (colision) {
    colision = false;

    position = position_new_randompos(board->width, board->height);

    body_piece_t* next = board->snake1->body_start;
    if (snake_get_next_body_piece(next) == NULL) break;
    next = snake_get_next_body_piece(next);
    while (next != NULL) {
      if (position->x == snake_get_body_piece_position(next)->x && position->y == snake_get_body_piece_position(next)->y) {
        colision = true;
        break;
      }
        next = snake_get_next_body_piece(next);
    }
  }
  fruit_set_position(fruit, position);
}

void
fruit_step(fruit_t* fruit, board_t* board)
{
  if ((snake_get_head_position(board->snake1)->x == fruit->position->x &&
  snake_get_head_position(board->snake1)->y == fruit->position->y)) 
  // || (snake_get_head_position(board->snake2)->x == fruit->position->x && 
  // snake_get_head_position(board->snake2)->y == fruit->position->y)) {
    fruit_update_pos(fruit, board);
}


