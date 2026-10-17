#include "../include/snake_model.h"

struct body_piece {
    body_piece_t* next_piece;
    body_piece_t* prev_piece;
    position_t* position;
};

body_piece_t*
body_piece_new(position_t* position)
{
  body_piece_t* new_body_piece = (body_piece_t*)malloc(sizeof(body_piece_t));
  if (new_body_piece == NULL) return NULL;

  new_body_piece->position = position;
  new_body_piece->next_piece = NULL;
  new_body_piece->prev_piece = NULL;

  return new_body_piece;
}

void
body_piece_delete(body_piece_t* body_piece) 
{
  free(body_piece);
}

snake_t* 
snake_new(position_t* position)
{
  snake_t* new_snake = (snake_t*)malloc(sizeof(snake_t));
  if (new_snake == NULL) return NULL;

  new_snake->size = 1;
  new_snake->body_start = body_piece_new(position);
  new_snake->body_end = new_snake->body_start;
  new_snake->head_direction = NORTH;

  return new_snake;
}

void
snake_delete(snake_t* snake)
{
  body_piece_t* next = snake->body_start;
  while (next != NULL) {
    body_piece_t* following = next->next_piece;
    body_piece_delete(next);
    next = following;
  }

  free(snake);
}

void 
snake_increase_size(snake_t* snake, position_t* position)
{
  snake->size++;
  body_piece_t* new_head = body_piece_new(position);
  new_head->next_piece = snake->body_start;
  snake->body_start->prev_piece = new_head;

  snake->body_start = new_head;
}

// int 
// snake_decrease_size(snake_t* snake)
// {
//   body_piece_t* new_snake_head = snake->body_start; 
//   new_snake_head = new_snake_head->next_piece; 
//   new_snake_head->prev_piece = NULL; 
//   //If snake tail = snake head , then reached "Spawn Size"
//   if(new_snake_head == snake->body_end){
//       return 1; 
//   }
//   else{
//     body_piece_delete(snake->body_start); 
//     snake->body_start = new_snake_head;
//   }

//   return 0; 
// }

void 
snake_set_position(snake_t* snake, position_t* position)
{
  body_piece_t* new_head = body_piece_new(position);
  new_head->next_piece = snake->body_start;
  snake->body_start->prev_piece = new_head;

  snake->body_start = new_head;

  // pop back
  snake->body_end->prev_piece->next_piece = NULL;
  body_piece_t* new_end = snake->body_end->prev_piece;
  body_piece_delete(snake->body_end);
  snake->body_end = new_end;

}

position_t* 
snake_get_head_position(snake_t* snake)
{
  return snake->body_start->position;
}

body_piece_t* 
snake_get_next_body_piece(body_piece_t* body_piece)
{
  return body_piece->next_piece;
}

position_t* 
snake_get_body_piece_position(body_piece_t* body_piece)
{
  return body_piece == NULL ? NULL : body_piece->position;
}
