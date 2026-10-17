#include "../include/wall_view.h"

int 
wall_draw(wall_t* wall)
{
  if (vg_draw_rectangle(wall->position->x * 20, wall->position->y * 20, 20, 20, 0x00ff00)) return 1;
  return 0;
}
