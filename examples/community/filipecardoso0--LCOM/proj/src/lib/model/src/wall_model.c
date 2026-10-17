#include "../include/wall_model.h"

wall_t* 
wall_new(position_t* position)
{
  wall_t* new_wall = (wall_t*)malloc(sizeof(wall_t));
  if (new_wall == NULL) return NULL;

  new_wall->position = position;

  return new_wall;
}

void 
wall_delete(wall_t* wall)
{
  free(wall);
}

void 
wall_set_position(wall_t* wall, position_t* position)
{
  wall->position = position;
}

position_t* 
wall_get_position(wall_t* wall)
{
  return wall->position;
}
