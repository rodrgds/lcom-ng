#include "../include/snake_view.h"

int 
snake_draw(snake_t* snake)
{
  body_piece_t* next = snake->body_start;
  while (next != NULL) {
    position_t* position = snake_get_body_piece_position(next);
    next = snake_get_next_body_piece(next);
    if (vg_draw_rectangle(position->x * 20, position->y * 20, 20, 20, 0xffffff)) return 1;
  }
  return 0;
}
