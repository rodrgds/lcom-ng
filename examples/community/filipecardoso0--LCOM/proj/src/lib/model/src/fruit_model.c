#include "../include/fruit_model.h"

fruit_t* 
fruit_new(position_t* position)
{
  fruit_t* new_fruit = (fruit_t*)malloc(sizeof(fruit_t));
  if (new_fruit == NULL) return NULL;

  new_fruit->position = position;

  return new_fruit;
}

void 
fruit_delete(fruit_t* fruit)
{
  free(fruit);
}

void 
fruit_set_position(fruit_t* fruit, position_t* position)
{
  fruit->position = position;
}

position_t* 
fruit_get_position(fruit_t* fruit)
{
  return fruit->position;
}
